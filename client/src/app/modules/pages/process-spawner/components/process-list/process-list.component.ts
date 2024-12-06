import { ChangeDetectionStrategy, Component, OnInit } from '@angular/core';
import { MainConfig } from '../../../../shared/element-list/models/ElementListConfig';
import { DataSource } from '../../../../shared/element-list/models/data-source';
import { DisplayedColumns } from '../../../../shared/element-list/models/displayed-columns';
import { defaultColumns, defaultConfig, ELEMENT_DATA } from '../../../../shared/element-list/models/defaultConfig';
import { HandelContentEvent } from '../../../../shared/element-list/models/HandelContentEvent';
import { SpawnProcessCrudService } from '../../services/process-spawn-commuincation.service';
import { ActionType } from '../../../../shared/action-button/models/action-button-config';
import { ProcessSpawnRequestDto } from '../../models/process-spawn-request-dto';
import { catchError, first, of, tap } from 'rxjs';
import { shared } from '../../../../shared/shared';
import { ProcessSpawnResponseDto } from '../../models/process-spawn-response-dto';
import { ProcessSpawnService } from '../../services/process-spawn.service';
import { ProcessInstanceDto } from '../../models/process-instance-dto';
import { ProcessInstancesHolderService } from '../../services/process-instances-holder.service';
import { ProcessStatus } from '../../enums/process-status.enum';
// import { NotificationService } from '../../../../shared/services/notification.service';

@Component({
  selector: 'app-process-list',
  templateUrl: './process-list.component.html',
  styleUrl: './process-list.component.scss',
})
export class ProcessListComponent implements OnInit {
    columns: DisplayedColumns = [
        { name: 'index', displayName: 'No.' },
        { name: 'processType', displayName: 'Type' },
        { name: 'processId', displayName: 'Process Id' },
        { name: 'status', displayName: 'Process Status' }
    ];

    config: MainConfig = defaultConfig;
    dataSource: DataSource[] = new Array<DataSource>;
    
    constructor(
        private processSpawnService: ProcessSpawnService, 
        private processInstancesHolderSerivce: ProcessInstancesHolderService) {
    }
    
    ngOnInit(): void {
        this.processInstancesHolderSerivce.items$.subscribe((processInstances: ProcessInstanceDto[]) => {
            this.dataSource = processInstances.map((processInstance: ProcessInstanceDto) => {
                return {
                    index: processInstance.id,
                    processType: processInstance.processType,
                    processId: processInstance.processId,
                    status: ProcessStatus[processInstance.status]
                };
            });
        });

        this.getAllProcesses();
    }

    eventHandler($event: HandelContentEvent) {
        if ($event.actionType === ActionType.Create) {
            this.createProcess();
        } else if ($event.actionType === ActionType.Delete) {
            if (!shared.isNotNullOrUndefined($event.content) || $event.content.length === 0) {
                return;
            }

            this.delete($event.content[0].index);
        }
    }

    createProcess() {
        const errorHandler = (response: shared.response.Object<any>) => {
            // this.notificationService.error(response.errorMessage);
        };

        const spawnProcessRequest : ProcessSpawnRequestDto = {
            process_type: 'test',
            parameters: new Map<string, string>([
                ['param1', 'param1']
            ])
        };

        this.processSpawnService.create(spawnProcessRequest, errorHandler).pipe(
            first(),
            tap((spawnProcessResponse: ProcessInstanceDto | undefined) => {
                if (shared.isNotNullOrUndefined(spawnProcessResponse)) {
                    this.processInstancesHolderSerivce.addItem(spawnProcessResponse);
                }
            })
        ).subscribe();
    }

    getAllProcesses() {
        const errorHandler = (response: shared.response.Object<any>) => {
            // this.notificationService.error(response.errorMessage);
        };

        this.processSpawnService.getAll().pipe(
            first(),
            tap((processInstances: ProcessInstanceDto[] | undefined) => {
                if (shared.isNotNullOrUndefined(processInstances)) {
                    this.processInstancesHolderSerivce.setItems(processInstances);
                }
            })
        ).subscribe();
    }

    delete(id: number) {
        const errorHandler = (response: shared.response.Object<any>) => {
            // this.notificationService.error(response.errorMessage);
        };

        this.processSpawnService.delete(id, errorHandler).pipe(
            first(),
            tap((processInstance: ProcessInstanceDto | undefined) => {
                if (shared.isNotNullOrUndefined(processInstance)) {
                    this.processInstancesHolderSerivce.deleteById(processInstance.id);
                }
            })
        ).subscribe();
    }
}

import { Component, OnInit } from '@angular/core';
import { MainConfig } from '../../../../shared/element-list/models/ElementListConfig';
import { DataSource } from '../../../../shared/element-list/models/data-source';
import { DisplayedColumns } from '../../../../shared/element-list/models/displayed-columns';
import { HandelContentEvent } from '../../../../shared/element-list/models/HandelContentEvent';
import { ActionType, Type } from '../../../../shared/action-button/models/action-button-config';
import { isProcessSpawnRequestDto, ProcessSpawnRequestDto, toProcessSpawnRequestDto } from '../../models/process-spawn-request-dto';
import { catchError, first, mergeMap, of, tap, timer } from 'rxjs';
import { shared } from '../../../../shared/shared';
import { ProcessSpawnService } from '../../services/process-spawn.service';
import { ProcessInstanceDto } from '../../models/process-instance-dto';
import { ProcessInstancesHolderService } from '../../services/process-instances-holder.service';
import { ProcessStatus } from '../../enums/process-status.enum';
import { ActivatedRoute, Router } from '@angular/router';
import { PageConfig } from '../../../../shared/element-list/models/pageConfig';
import { MatDialog } from '@angular/material/dialog';
import { DialogComponent } from '../../../../shared/dialog/dialog.component';
import { ProcessInstanceCreateComponent } from '../process-instance-create/process-instance-create.component'
import { ProcessType } from '../../enums/process-type.enum';

@Component({
  selector: 'app-process-list',
  templateUrl: './process-list.component.html',
  styleUrl: './process-list.component.scss',
})
export class ProcessListComponent implements OnInit {
    id!: number;
    isUserPath = false; 
    isManagerPath = false; 
    
    tableConfig: MainConfig = {
      useFilters: false,
      filterConfig: {},
      contentConfig: {
        tableConfig: {
          multiselect: true,
          rowTableIcons: [
            {
                name: 'Show',
                displayName: 'Events',
                actionType: ActionType.SpecialAction2,
                type: Type.Default,
                iconName: 'preview'
            }
          ],
          multiActionButton: {
            actions: [
            /*
            {
                name: 'edit',
                displayName: 'Edit',
                actionType: ActionType.Edit,
                type: Type.Default,
                iconName: 'edit-outline'
            },
            */
            {
              name: 'delete',
              displayName: 'Del',
              actionType: ActionType.Delete,
              type: Type.Error,
              iconName: 'trash-can-outline'
            },
            {
              name: 'finish_process',
              displayName: 'Finish',
              actionType: ActionType.SpecialAction1,
              type: Type.Default,
              iconName: 'start'
            }
            ]
        },
          sortable: true,
          dbClickEdit: true,
          stickyHeader: true,
        },
        usePagination: true,
        paginationConfig: {
            pageConfig: {
                length: 200,
                pageIndex: 0,
                size: 10
            },
            pageSizeOptions: [5, 10, 20, 50]
        },
        useContentHandler: true,
        contentHandlerConfig: { buttons: [{
            text: 'Create',
            disabled: false,
            type: Type.Default,
            actionType: ActionType.Create
          }, {
            text: 'Edit',
            disabled: true,
            type: Type.Info,
            actionType: ActionType.Edit
          }, {
            text: 'Reload',
            disabled: true,
            type: Type.Warn,
            actionType: ActionType.Reload
          }, {
            text: 'Delete',
            disabled: false,
            type: Type.Error,
            actionType: ActionType.Delete
          }]
        }
      }
    };

    columns: DisplayedColumns = [
        { name: 'index', displayName: 'No.' },
        { name: 'processType', displayName: 'Type' },
        { name: 'processId', displayName: 'Process Id' },
        { name: 'status', displayName: 'Process Status' }
    ];

    config: MainConfig = this.tableConfig;
    dataSource: DataSource[] = new Array<DataSource>;

    constructor(
        private router: Router,
        private route: ActivatedRoute,
        private processSpawnService: ProcessSpawnService, 
        private processInstancesHolderSerivce: ProcessInstancesHolderService) {
    }
    
    ngOnInit(): void {
        if (!this.loadPath()) {
            this.goDashboard();
        }

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

        this.loadData(1, this.config.contentConfig.paginationConfig.pageConfig.size);
    }

    goDashboard() {
        this.router.navigate(['/dashboard']);
    }

    loadPath(): boolean {
        this.id = Number(this.route.snapshot.paramMap.get('id'));
        const pathSegments = this.route.snapshot.url.map(segment => segment.path);
        if (pathSegments.includes('user')) {
            this.isUserPath = true;

            return true;
        } else if (pathSegments.includes('manager')) {
            this.isManagerPath = true;
            
            return true;
        } else {
            return false;
        }
    }

    loadData(pageNumber: number, pageSize: number) {
        if (this.isUserPath) {
            this.getForUser(pageNumber, pageSize);
        } else if (this.isManagerPath) {
            this.getForProcessManager(pageNumber, pageSize);
        } else {
            this.goDashboard();
        }
    }

    getForUser(pageNumber: number, pageSize: number) {
        const errorHandler = (response: shared.response.Object<any>) => {
            // this.notificationService.error(response.errorMessage);
        };

        this.processSpawnService.getProcessesByUserId(this.id, pageNumber, pageSize, errorHandler).pipe(
            first(),
            tap((processInstances: shared.response.BasePagination<ProcessInstanceDto> | undefined) => {
                if (shared.isNotNullOrUndefined(processInstances)) {
                    this.processInstancesHolderSerivce.setItems(processInstances.data);
                    this.config.contentConfig.paginationConfig.pageConfig.size = processInstances.pageSize;
                    this.config.contentConfig.paginationConfig.pageConfig.pageIndex = processInstances.pageNumber - 1;
                    this.config.contentConfig.paginationConfig.pageConfig.length = processInstances.total;
                }
            })
        ).subscribe();
    }

    getForProcessManager(pageNumber: number, pageSize: number) {
        const errorHandler = (response: shared.response.Object<any>) => {
            // this.notificationService.error(response.errorMessage);
        };

        this.processSpawnService.getProcessByManagerId(this.id, pageNumber, pageSize, errorHandler).pipe(
            first(),
            tap((processInstances: shared.response.BasePagination<ProcessInstanceDto> | undefined) => {
                if (shared.isNotNullOrUndefined(processInstances)) {
                    this.processInstancesHolderSerivce.setItems(processInstances.data);
                    this.config.contentConfig.paginationConfig.pageConfig.size = processInstances.pageSize;
                    this.config.contentConfig.paginationConfig.pageConfig.pageIndex = processInstances.pageNumber - 1;
                    this.config.contentConfig.paginationConfig.pageConfig.length = processInstances.total;
                }
            })
        ).subscribe();
    }

    goProcessEvents(processId: number) {
        this.router.navigate(['/process-event/process/' + processId]);
    }

    eventHandler($event: HandelContentEvent) {
        if ($event.actionType === ActionType.Create) {
            this.create();
        } else if ($event.actionType === ActionType.Delete) {
            if (!shared.isNotNullOrUndefined($event.content) || $event.content.length === 0) {
                return;
            }

            this.delete($event.content[0].index);
        } else if ($event.actionType === ActionType.SpecialAction1) {
            if (!shared.isNotNullOrUndefined($event.content) || $event.content.length === 0) {
                return;
            }

            this.finishProcess($event.content[0].index);
        } else if ($event.actionType === ActionType.SpecialAction2) {
            const content = $event.content;
            if (content.length != 1) {
                return;
            }

            this.goProcessEvents(content[0].index);
        }
    }

    create() {
        const errorHandler = (response: shared.response.Object<any>) => {
            // this.notificationService.error(response.errorMessage);
        };

        this.processSpawnService.createProcess(errorHandler).pipe(
            first(),
            tap((spawnProcessResponse: ProcessInstanceDto | undefined) => {
                if (shared.isNotNullOrUndefined(spawnProcessResponse)) {
                    this.processInstancesHolderSerivce.addItem(spawnProcessResponse);
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

    finishProcess(id: number) {
        const errorHandler = (response: shared.response.Object<any>) => {
            // this.notificationService.error(response.errorMessage);
        };

        this.processSpawnService.finishProcess(id, errorHandler).pipe(
            first(),
            tap((processInstance: ProcessInstanceDto | undefined) => {
                if (shared.isNotNullOrUndefined(processInstance)) {
                    this.processInstancesHolderSerivce.updateById(processInstance);
                }
            })
        ).subscribe();
    }
    
    onPageChange($event: PageConfig) {
        this.loadData($event.pageIndex + 1, $event.size);
    }
}

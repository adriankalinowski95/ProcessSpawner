import { Component } from '@angular/core';
import { DisplayedColumns } from '../../../../shared/element-list/models/displayed-columns';
import { MainConfig } from '../../../../shared/element-list/models/ElementListConfig';
import { DataSource } from '../../../../shared/element-list/models/data-source';
import { defaultConfig } from '../../../../shared/element-list/models/defaultConfig';
import { HandelContentEvent } from '../../../../shared/element-list/models/HandelContentEvent';
import { ActionType, Type } from '../../../../shared/action-button/models/action-button-config';
import { shared } from '../../../../shared/shared';
import { ProcessManagersHolderService } from '../../services/process-managers-holder.servcie';
import { ProcessManagerService } from '../../services/process-manager.service';
import { isProcessManagerDto, ProcessManagerDto } from '../../models/process-manager-dto';
import { first, tap } from 'rxjs';
import { ActivatedRoute, Router } from '@angular/router';
import { PageConfig } from '../../../../shared/element-list/models/pageConfig';
import { ProcessInstanceDto } from 'app/modules/pages/process-spawner/models/process-instance-dto';

@Component({
  selector: 'app-process-manager-list',
  templateUrl: './process-manager-list.component.html',
  styleUrl: './process-manager-list.component.scss'
})
export class ProcessManagerListComponent {
    tableConfig: MainConfig = {
          useFilters: false,
          filterConfig: {},
          contentConfig: {
            tableConfig: {
              multiselect: true,
              rowTableIcons: [
                {
                    name: 'Show',
                    displayName: 'Proceses',
                    actionType: ActionType.SpecialAction1,
                    type: Type.Default,
                    iconName: 'preview'
                }
              ],
              multiActionButton: {
                actions: [{
                    name: 'edit',
                    displayName: 'Edit',
                    actionType: ActionType.Edit,
                    type: Type.Default,
                    iconName: 'edit-outline'
                },
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
                disabled: false,
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
        { name: 'name', displayName: 'Name' },
        { name: 'address', displayName: 'Ip' },
        { name: 'port', displayName: 'Port' }
    ];
    
    config: MainConfig = this.tableConfig;
    dataSource: DataSource[] = new Array<DataSource>;

    constructor(
        private router: Router,
        private processManagerService: ProcessManagerService, 
        private processManagersHolderSerivce: ProcessManagersHolderService) {}
        
    ngOnInit(): void {
        this.processManagersHolderSerivce.items$.subscribe((processInstances: ProcessManagerDto[]) => {
            this.dataSource = processInstances.map((processInstance: ProcessManagerDto) => {
                return {
                    index: processInstance.id,
                    name: processInstance.name,
                    address: processInstance.address,
                    port: processInstance.port
                };
            });
        });

        this.getAllProcessManagers();
    }

    eventHandler($event: HandelContentEvent) {
        if ($event.actionType === ActionType.Create) {
            this.create();
        } else if ($event.actionType === ActionType.Edit) {
            if (!shared.isNotNullOrUndefined($event.content) || $event.content.length === 0) {
                return;
            }

            this.update($event.content[0].index);
        }
        else if ($event.actionType === ActionType.Delete) {
            if (!shared.isNotNullOrUndefined($event.content) || $event.content.length === 0) {
                return;
            }

            this.update($event.content[0].index);
        } else if ($event.actionType === ActionType.SpecialAction1) {
            const content = $event.content;
            if (content.length != 1) {
                return;
            }

            this.goProcesses(content[0].index);
        }
    }

    create() {
        this.processManagerService.createProcessManager().pipe(
                    first(),
                    tap((processManagerDto: ProcessManagerDto | undefined) => {
                        if (!shared.isNotNullOrUndefined(processManagerDto)) {
                            return;
                        }
                        if (!isProcessManagerDto(processManagerDto)) {
                            return;
                        }

                        this.processManagersHolderSerivce.addItem(processManagerDto);
                    })).subscribe();
    }

    update(id: number) {
        const processManager = this.processManagersHolderSerivce.getById(id);
        if (!shared.isNotNullOrUndefined(processManager)) {
            // this.notificationService.error('Process manager not found');
            return;
        }

        this.processManagerService.updateProcessManager(processManager).pipe(
            first(),
            tap((processManagerDto: ProcessManagerDto | undefined) => {
                if (!shared.isNotNullOrUndefined(processManagerDto)) {
                    return;
                }
                
                if (!isProcessManagerDto(processManagerDto)) {
                    return;
                }

                this.processManagersHolderSerivce.updateById(processManagerDto);
            })).subscribe();
    }

    delete(id: number) {
        const errorHandler = (response: shared.response.Object<any>) => {
            // this.notificationService.error(response.errorMessage);
        };

        this.processManagerService.delete(id, errorHandler).pipe(
            first(),
            tap((processInstance: ProcessManagerDto | undefined) => {
                if (shared.isNotNullOrUndefined(processInstance)) {
                    this.processManagersHolderSerivce.deleteById(processInstance.id);
                }
            })
        ).subscribe();
    }

    goProcesses(managerId: number) {
        this.router.navigate(['/process-spawner/manager/' + managerId]);
    }

    getAllProcessManagers() {
        const errorHandler = (response: shared.response.Object<any>) => {
            // this.notificationService.error(response.errorMessage);
        };

        this.processManagerService.getAll().pipe(
            first(),
            tap((processManagers: ProcessManagerDto[] | undefined) => {
                if (shared.isNotNullOrUndefined(processManagers)) {
                    this.processManagersHolderSerivce.setItems(processManagers);
                    this.config.contentConfig.paginationConfig.pageConfig.length = processManagers.length;
                }
            })
        ).subscribe();
    }

    onPageChange($event: PageConfig) {
        console.log('pageChange', $event)
    }
}

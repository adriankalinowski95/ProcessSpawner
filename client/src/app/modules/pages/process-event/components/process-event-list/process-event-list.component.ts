import { Component } from '@angular/core';
import { MainConfig } from '../../../../shared/element-list/models/ElementListConfig';
import { DisplayedColumns } from '../../../../shared/element-list/models/displayed-columns';
import { ActionType, Type } from '../../../../shared/action-button/models/action-button-config';
import { DataSource } from '../../../../shared/element-list/models/data-source';
import { ActivatedRoute, Router } from '@angular/router';
import { ProcessEventService } from '../../services/process-event.service';
import { ProcessEventsHolderService } from '../../services/process-events-holder.servcie';
import { ProcessEventDto } from '../../models/process-manager-dto';
import { HandelContentEvent } from '../../../../shared/element-list/models/HandelContentEvent';
import { shared } from '../../../../shared/shared';
import { first, tap } from 'rxjs';
import { PageConfig } from '../../../../shared/element-list/models/pageConfig';

@Component({
  selector: 'app-process-event-list',
  templateUrl: './process-event-list.component.html',
  styleUrl: './process-event-list.component.scss'
})
export class ProcessEventListComponent {
    id!: number;
    isPathCorrect: boolean;
    
    tableConfig: MainConfig = {
      useFilters: false,
      filterConfig: {},
      contentConfig: {
        tableConfig: {
          multiselect: true,
          rowTableIcons: [],
          multiActionButton: {
            actions: [
                {
                  name: 'delete',
                  displayName: 'Delete',
                  actionType: ActionType.Delete,
                  type: Type.Error,
                  iconName: 'trash-can-outline'
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
        contentHandlerConfig: { 
            buttons: [
                {
                    text: 'Delete',
                    disabled: false,
                    type: Type.Error,
                    actionType: ActionType.Delete
                }
            ]
        }
      }
    };

    columns: DisplayedColumns = [
        { name: 'index', displayName: 'No.' },
        { name: 'name', displayName: 'Name' },
        { name: 'value', displayName: 'Value' },
        { name: 'createdTime', displayName: 'CreatedTime' }
    ];

    config: MainConfig = this.tableConfig;
    dataSource: DataSource[] = new Array<DataSource>;
    
    constructor(
        private router: Router,
        private route: ActivatedRoute,
        private processEventService: ProcessEventService, 
        private processEventsHolderSerivce: ProcessEventsHolderService) {
    }
    
    ngOnInit(): void {
        if (!this.loadPath()) {
            this.goDashboard();
        }

        this.processEventsHolderSerivce.items$.subscribe((processEvents: ProcessEventDto[]) => {
            this.dataSource = processEvents.map((processEvent: ProcessEventDto) => {
                return {
                    index: processEvent.id,
                    name: processEvent.name,
                    value: processEvent.value,
                    createdTime: processEvent.createdTimeMs.toString()
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
        if (pathSegments.includes('process')) {
            this.isPathCorrect = true;

            return true;
        }

        return false;
    }

    eventHandler($event: HandelContentEvent) {
       if ($event.actionType === ActionType.Delete) {
           /*
           if (!shared.isNotNullOrUndefined($event.content) || $event.content.length === 0) {
               return;
           }
           this.delete($event.content[0].index);
           */
       }
    }

    loadData(pageIndex: number, pageSize: number) {
        if (this.isPathCorrect) {
            this.getForProcess(pageIndex, pageSize);
        } else {
            this.goDashboard();
        }
    }

    getForProcess(pageNumber: number, pageSize: number) {
        const errorHandler = (response: shared.response.Object<any>) => {
            // this.notificationService.error(response.errorMessage);
        };

        this.processEventService.getProcessEventsByUserId(this.id, pageNumber, pageSize, errorHandler).pipe(
            first(),
            tap((processEvents: shared.response.BasePagination<ProcessEventDto> | undefined) => {
                if (shared.isNotNullOrUndefined(processEvents)) {
                    this.processEventsHolderSerivce.setItems(processEvents.data);
                    this.config.contentConfig.paginationConfig.pageConfig.size = processEvents.pageSize;
                    this.config.contentConfig.paginationConfig.pageConfig.pageIndex = processEvents.pageNumber - 1;
                    this.config.contentConfig.paginationConfig.pageConfig.length = processEvents.total;
                }
            })
        ).subscribe();
    }

    onPageChange($event: PageConfig) {
        this.loadData($event.pageIndex + 1, $event.size);
    }
}

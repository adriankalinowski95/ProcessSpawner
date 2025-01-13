import { Injectable } from "@angular/core";
import { isProcessSpawnRequestDto, ProcessSpawnRequestDto } from "../models/process-spawn-request-dto";
import { BehaviorSubject, catchError, finalize, first, map, Observable, of, switchMap, tap } from "rxjs";
import { ProcessInstanceDto } from "../models/process-instance-dto";
import { shared } from "../../../shared/shared";
import { ProcessSpawnActionsService } from "./process-spawn-actions.service";
import { ProcessInstanceCrudService } from "./process-instance-crud.serivce";
import { ExtendedCrudServiceImpl } from "../../../shared/request/extended-crud-service-impl";
import { DialogComponent } from "app/modules/shared/dialog/dialog.component";
import { ProcessInstanceCreateComponent } from "../components/process-instance-create/process-instance-create.component";
import { ActionType, Type } from "app/modules/shared/action-button/models/action-button-config";
import { MatDialog } from "@angular/material/dialog";

@Injectable({
    providedIn: 'root'
})
export class ProcessSpawnService extends ExtendedCrudServiceImpl<ProcessInstanceDto> {
    constructor(private dialog: MatDialog,
                private processInstanceCrudService: ProcessInstanceCrudService,
                private processSpawnActionsService: ProcessSpawnActionsService) 
    {
        super(processInstanceCrudService);
    }

    private spawnProcess(processSpawnRequest: ProcessSpawnRequestDto, responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);
        
        return this.processSpawnActionsService.spawnProcess(processSpawnRequest).pipe(
            first(),
            map((response: shared.response.Object<ProcessInstanceDto>) => {
                if (response.status != shared.enums.BaseResponseStatus.Ok) {
                    if (!responseCallback) {
                        return undefined;
                    }

                    throw new Error(response.errorMessage);
                }

                return response.object;
            }),
            catchError((err) => {
                if (responseCallback) {
                    responseCallback({
                        errorMessage: err.message, 
                        status: shared.enums.BaseResponseStatus.Error,
                        object: null
                    });
                }

                return of(undefined);
            }),
            finalize(() => {
                this.isLoadingSubject.next(false)
            }));
    }

    finishProcess(id: number, responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);

        return this.processSpawnActionsService.finishProcess(id).pipe(
            first(),
            map((response: shared.response.Object<ProcessInstanceDto>) => {
                if (response.status != shared.enums.BaseResponseStatus.Ok) {
                    if (!responseCallback) {
                        return undefined;
                    }

                    throw new Error(response.errorMessage);
                }

                return response.object;
            }),
            catchError((err) => {
                if (responseCallback) {
                    responseCallback({
                        errorMessage: err.message, 
                        status: shared.enums.BaseResponseStatus.Error,
                        object: null
                    });
                }

                return of(undefined);
            }),
            finalize(() => {
                this.isLoadingSubject.next(false)
            }));
    }

    getProcessesByUserId(userId: string, pageNumber: number, pageSize: number, responseCallback?: (response: shared.response.Object<any>) => void) {
        return this.getWithPaginationTemplate(pageNumber, pageSize, (pageNumber: number, pageSize: number) => {
            return this.processInstanceCrudService.getByUserId(userId, pageNumber, pageSize);
        }, responseCallback);
    }

    getProcessByManagerId(managerId: number, pageNumber: number, pageSize: number, responseCallback?: (response: shared.response.Object<any>) => void) {
        return this.getWithPaginationTemplate(pageNumber, pageSize, (pageNumber: number, pageSize: number) => {
            return this.processInstanceCrudService.getByManager(managerId, pageNumber, pageSize);
        }, responseCallback);
    }

    createProcess(responseCallback?: (response: shared.response.Object<any>) => void) {
        return this.dialog.open(DialogComponent<ProcessInstanceCreateComponent>, {
                    width: '650px', 
                    height: '650px',
                    data: {
                      name: 'create_process',
                      header: 'Create Process',
                      contentData: '',
                      contentRef: ProcessInstanceCreateComponent,
                      config: {
                        actionPanelButtons: [
                          {
                            disabled: false,
                            type: Type.Info,
                            text: 'Accept',
                            actionType: ActionType.Save
                          },
                          {
                            disabled: false,
                            type: Type.Error,
                            text: 'Close',
                            actionType: ActionType.Close
                          }
                        ]
                      }
                    }
                  }).afterClosed().pipe(
                    first(),
                    switchMap(processSpawnRequest => { 
                        if (!isProcessSpawnRequestDto(processSpawnRequest)) {
                            if (!responseCallback) {
                                return of(undefined);
                            }
        
                            throw new Error("Data from dialog is incorrect");
                        }
        
                        return this.spawnProcess(processSpawnRequest);
                    }),
                    catchError((err) => {
                        if (responseCallback) {
                            responseCallback({
                                errorMessage: err.message, 
                                status: shared.enums.BaseResponseStatus.Error,
                                object: null
                            });
                        }
                    
                        return of(undefined);
                    })
                  );
    }
}
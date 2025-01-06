import { Injectable } from "@angular/core";
import { ProcessSpawnRequestDto } from "../models/process-spawn-request-dto";
import { BehaviorSubject, catchError, finalize, first, map, Observable, of, tap } from "rxjs";
import { ProcessInstanceDto } from "../models/process-instance-dto";
import { shared } from "../../../shared/shared";
import { ProcessSpawnActionsService } from "./process-spawn-actions.service";
import { BaseCrudServiceImpl } from "../../../shared/request/base-crud-impl-service";
import { ProcessInstanceCrudService } from "./process-instance-crud.serivce";
import { ExtendedCrudServiceImpl } from "../../../shared/request/extended-crud-service-impl";

@Injectable({
    providedIn: 'root'
})
export class ProcessSpawnService extends ExtendedCrudServiceImpl<ProcessInstanceDto> {
    constructor(private processInstanceCrudService: ProcessInstanceCrudService,
                private processSpawnActionsService: ProcessSpawnActionsService) 
    {
        super(processInstanceCrudService);
    }

    spawnProcess(processSpawnRequest: ProcessSpawnRequestDto, responseCallback?: (response: shared.response.Object<any>) => void) {
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

    getProcessesByUserId(userId: number, pageNumber: number, pageSize: number, responseCallback?: (response: shared.response.Object<any>) => void) {
        return this.getWithPaginationTemplate(pageNumber, pageSize, (pageNumber: number, pageSize: number) => {
            return this.processInstanceCrudService.getByUserId(userId, pageNumber, pageSize);
        }, responseCallback);
    }

    getProcessByManagerId(managerId: number, pageNumber: number, pageSize: number, responseCallback?: (response: shared.response.Object<any>) => void) {
        return this.getWithPaginationTemplate(pageNumber, pageSize, (pageNumber: number, pageSize: number) => {
            return this.processInstanceCrudService.getByManager(managerId, pageNumber, pageSize);
        }, responseCallback);
    }
}
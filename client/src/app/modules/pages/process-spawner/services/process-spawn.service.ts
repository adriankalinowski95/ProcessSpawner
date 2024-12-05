import { Injectable } from "@angular/core";
import { ProcessSpawnCrudService } from "./process-spawn-crud.service";
import { ProcessSpawnRequestDto } from "../models/process-spawn-request-dto";
import { BehaviorSubject, catchError, finalize, first, map, Observable, of, tap } from "rxjs";
import { ProcessInstanceDto } from "../models/process-instance-dto";
import { shared } from "../../../shared/shared";

@Injectable({
    providedIn: 'root'
})
export class ProcessSpawnService {
    isLoadingSubject: BehaviorSubject<boolean>;
    isLoading$: Observable<boolean>;

    constructor(private processSpawnCrudService: ProcessSpawnCrudService) {
        this.isLoadingSubject = new BehaviorSubject<boolean>(false);
        this.isLoading$ = this.isLoadingSubject.asObservable();
    }

    create(processSpawnRequest: ProcessSpawnRequestDto, responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);
        
        return this.processSpawnCrudService.spawnProcess(processSpawnRequest).pipe(
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

    getAll(responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);

        return this.processSpawnCrudService.getAll().pipe(
            first(),
            map((response: shared.response.Objects<ProcessInstanceDto>) => {
                if (response.status != shared.enums.BaseResponseStatus.Ok) {
                    throw new Error(response.errorMessage);
                }

                return response.objects;
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

    delete(id: number, responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);

        return this.processSpawnCrudService.delete(id).pipe(
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
}
import { BehaviorSubject, catchError, finalize, first, map, Observable, of } from "rxjs";
import { BaseCrudService } from "./base-crud-service";
import { shared } from "../shared";

export class BaseCrudServiceImpl<T extends { id: any }> {
    isLoadingSubject: BehaviorSubject<boolean>;
    isLoading$: Observable<boolean>;
    
    public constructor(private crudService: BaseCrudService<T>) {
        this.isLoadingSubject = new BehaviorSubject<boolean>(false);
        this.isLoading$ = this.isLoadingSubject.asObservable();
    }

    public getAll(responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);

        return this.crudService.getAll().pipe(
            first(),
            map((response: shared.response.Objects<T>) => {
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

    public create(model: T, responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);
        
        return this.crudService.create(model).pipe(
            first(),
            map((response: shared.response.Object<T>) => {
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

    public update(model: T, responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);
        
        return this.crudService.update(model).pipe(
            first(),
            map((response: shared.response.Object<T>) => {
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

    public delete(id: number, responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);

        return this.crudService.delete(id).pipe(
            first(),
            map((response: shared.response.Object<T>) => {
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
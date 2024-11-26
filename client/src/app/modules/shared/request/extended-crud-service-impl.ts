import { catchError, finalize, first, map, Observable, of } from "rxjs";
import { BaseCrudServiceImpl } from "./base-crud-impl-service";
import { ExtendedCrudService } from "./extended-crud-service";
import { shared } from "../shared";

export class ExtendedCrudServiceImpl<T extends { id: any }> extends BaseCrudServiceImpl<T> {
    constructor(private extendedCrudService: ExtendedCrudService<T>) {
        super(extendedCrudService);
    }
    
    public getWithPaginationTemplate(pageNumber: number = 1, pageSize: number = 20, requestProvider:(pageNumber: number, pageSize: number) => Observable<any> ,responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);
        
        return requestProvider(pageNumber, pageSize).pipe(
            first(),
            map((response: shared.response.BasePagination<T>) => {
                if (response.status != shared.enums.BaseResponseStatus.Ok) {
                    if (!responseCallback) {
                        return undefined;
                    }

                    throw new Error(response.errorMessage);
                }

                return response;
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

    public getPaginationConfig(
        params: Map<string, string>, 
        requestProvider:(params: Map<string, string>) => Observable<any>,
        responseCallback?: (response: shared.response.Object<any>) => void) {
        this.isLoadingSubject.next(true);
        
        return requestProvider(params).pipe(
            first(),
            map((response: shared.response.BasePaginationConfig) => {
                if (response.status != shared.enums.BaseResponseStatus.Ok) {
                    if (!responseCallback) {
                        return undefined;
                    }

                    throw new Error(response.errorMessage);
                }

                return response;
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
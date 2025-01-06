import { Observable } from "rxjs";
import { BaseCrudService } from "./base-crud-service";
import { HttpClient } from "@angular/common/http";


export class ExtendedCrudService<T extends { id: any }> extends BaseCrudService<T> {
    constructor(http: HttpClient, basePath: string) {
        super(http, basePath);
    }
    
    getWithPagination(pageNumber: number = 1, pageSize: number = 20): Observable<any> {
        return this.http.get(this.basePath, {
            params: {
                pageNumber: pageNumber,
                pageSize: pageSize
            }
        });
    }

    getPaginationConfig(): Observable<any> {
        return this.http.get(this.basePath + '/pagination-config');
    }
}
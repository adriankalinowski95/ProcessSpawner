import { HttpClient } from "@angular/common/http";
import { environment } from "../../../../../environments/environment.prod";
import { ProcessEventDto } from "../models/process-manager-dto";
import { ExtendedCrudService } from "../../../shared/request/extended-crud-service";
import { Injectable } from "@angular/core";
import { Observable } from "rxjs";

@Injectable({
    providedIn: 'root'
})
export class ProcessEventCrudService extends ExtendedCrudService<ProcessEventDto>{ 
    constructor(http: HttpClient) {
        super(http, environment.apiUrl + '/api/ProcessEvent');
    }

    getByProcessId(processId: number, pageNumber: number = 1, pageSize: number = 20): Observable<any> {
        const path = this.basePath + '/process/' + processId;
        return this.http.get(path, {
            params: {
                pageNumber: pageNumber,
                pageSize: pageSize
            }
        });
    }

    getPaginationConfigForProcess(processId: number): Observable<any> {
        return this.http.get(this.basePath + '/process/pagination-config/' + processId);
    }
}
  
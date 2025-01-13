import { Injectable } from "@angular/core";
import { environment } from "../../../../../environments/environment.prod";
import { BaseCrudService } from "../../../shared/request/base-crud-service";
import { ProcessInstanceDto } from "../models/process-instance-dto";
import { HttpClient } from "@angular/common/http";
import { ExtendedCrudService } from "../../../shared/request/extended-crud-service";
import { Observable } from "rxjs";
import { AuthService } from "../../../auth/services/auth.service";

@Injectable({
    providedIn: 'root'
})
export class ProcessInstanceCrudService extends ExtendedCrudService<ProcessInstanceDto> { 
    constructor(http: HttpClient, authService: AuthService) {
        super(http, environment.apiUrl + '/api/ProcessInstance');
    }

    getByUserId(userId: string, pageNumber: number = 1, pageSize: number = 20): Observable<any> {
        const path = this.basePath + '/user/' + userId;
        return this.http.get(path, {
            params: {
                pageNumber: pageNumber,
                pageSize: pageSize
            }
        });
    }

    getByManager(managerId: number, pageNumber: number = 1, pageSize: number = 20): Observable<any> {
        const path = this.basePath + '/manager/' + managerId;

        return this.http.get(path, {
            params: {
                pageNumber: pageNumber,
                pageSize: pageSize
            }
        });
    }
}
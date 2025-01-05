import { Injectable } from "@angular/core";
import { environment } from "../../../../../environments/environment.prod";
import { BaseCrudService } from "../../../shared/request/base-crud-service";
import { ProcessInstanceDto } from "../models/process-instance-dto";
import { HttpClient } from "@angular/common/http";

@Injectable({
    providedIn: 'root'
})
export class ProcessInstanceCrudService extends BaseCrudService<ProcessInstanceDto> { 
    constructor(http: HttpClient) {
        super(http, environment.apiUrl + '/api/ProcessInstance');
    }
}
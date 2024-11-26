import { Injectable } from "@angular/core";
import { environment } from "../../../../../environments/environment.prod";
import { HttpClient } from "@angular/common/http";
import { BaseCrudService } from "../../../shared/request/base-crud-service";
import { Observable } from "rxjs";
import { ProcessManagerDto } from "../models/process-manager-dto";

@Injectable({
    providedIn: 'root'
})
export class ProcessManagerCrudService extends BaseCrudService<ProcessManagerDto> { 
    constructor(http: HttpClient) {
        super(http, environment.apiUrl + '/api/ProcessManager');
    }
}
  
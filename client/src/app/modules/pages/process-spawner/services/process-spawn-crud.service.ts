import { Injectable } from "@angular/core";
import { environment } from "../../../../../environments/environment.prod";
import { HttpClient } from "@angular/common/http";
import { BaseCrudService } from "../../../shared/request/base-crud-service";
import { ProcessInstanceDto } from "../models/process-instance-dto";
import { ProcessSpawnRequestDto } from "../models/process-spawn-request-dto";
import { Observable } from "rxjs";

@Injectable({
    providedIn: 'root'
})
export class ProcessSpawnCrudService extends BaseCrudService<ProcessInstanceDto> { 
    constructor(http: HttpClient) {
        super(http, environment.apiUrl + '/api/ProcessSpawning');
    }

    spawnProcess(processSpawnRequest: ProcessSpawnRequestDto) : Observable<any> {
        return this.http.post(this.basePath, processSpawnRequest);
    }

    finishProcess(id: number) : Observable<any> {
        return this.http.post(this.basePath + '/finish-process/' + id, {});
    }
}
  
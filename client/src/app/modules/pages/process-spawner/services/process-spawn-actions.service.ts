import { HttpClient } from "@angular/common/http";
import { Injectable } from "@angular/core";
import { environment } from "../../../../../environments/environment.prod";
import { ProcessSpawnRequestDto } from "../models/process-spawn-request-dto";
import { Observable } from "rxjs";

@Injectable({
    providedIn: 'root'
})
export class ProcessSpawnActionsService {
    private PROCESS_SPAWNING_BASE_PATH = environment.apiUrl + '/api/ProcessSpawning';

    constructor(private http: HttpClient) { }
    
    spawnProcess(processSpawnRequest: ProcessSpawnRequestDto) : Observable<any> {
        const requestPayload = {
            process_type: processSpawnRequest.process_type,
            parameters: Object.fromEntries(processSpawnRequest.parameters)
        };
        
        return this.http.post(this.PROCESS_SPAWNING_BASE_PATH + "/spawn-process", requestPayload);
    }

    finishProcess(id: number) : Observable<any> {
        return this.http.post(this.PROCESS_SPAWNING_BASE_PATH + '/finish-process/' + id, {});
    }
}
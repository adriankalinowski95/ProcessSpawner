import { HttpClient } from "@angular/common/http";
import { Injectable } from "@angular/core";
import { environment } from "../../../../../environments/environment.prod";
import { ProcessSpawnRequestDto } from "../models/process-spawn-request-dto";
import { Observable } from "rxjs";

@Injectable({
    providedIn: 'root'
})
export class SpawnProcessCommunicationService {
    private PROCESS_SPAWNING_BASE_PATH = environment.apiUrl + '/api/ProcessSpawning';

    constructor(private http: HttpClient) { }
    
    spawnProcess(processSpawnRequest: ProcessSpawnRequestDto) : Observable<any> {
        return this.http.post(this.PROCESS_SPAWNING_BASE_PATH, processSpawnRequest);
    }

    finishProcess(id: number) : Observable<any> {
        return this.http.post(this.PROCESS_SPAWNING_BASE_PATH + '/finish-process/' + id, {});
    }
}
import { HttpClient } from "@angular/common/http";
import { Injectable } from "@angular/core";
import { environment } from "../../../../../environments/environment.prod";
import { SpawnProcessRequestDto } from "../models/spawn-process-request-dto";
import { Observable } from "rxjs";

@Injectable({
    providedIn: 'root'
})
export class SpawnProcessCrudService {
    private PROCESS_SPAWNING_BASE_PATH = environment.apiUrl + '/api/ProcessSpawning';

    constructor(private http: HttpClient) { }

    spawnProcess(spawnProcessRequest: SpawnProcessRequestDto): Observable<any> {
        return this.http.post(this.PROCESS_SPAWNING_BASE_PATH + '/SpawnProcess', spawnProcessRequest);
    }
}
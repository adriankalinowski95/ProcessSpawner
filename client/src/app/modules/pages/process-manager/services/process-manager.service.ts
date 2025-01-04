import { Injectable } from "@angular/core";
import { BaseCrudServiceImpl } from "../../../shared/request/base-crud-impl-service";
import { ProcessInstanceDto } from "../../process-spawner/models/process-instance-dto";
import { ProcessManagerDto } from "../models/process-manager-dto";
import { ProcessManagerCrudService } from "./process-manager-crud.service";

@Injectable({
    providedIn: 'root'
})
export class ProcessManagerService extends BaseCrudServiceImpl<ProcessManagerDto> {
    constructor(private processManagerCrudService: ProcessManagerCrudService) 
    {
        super(processManagerCrudService);
    }
}
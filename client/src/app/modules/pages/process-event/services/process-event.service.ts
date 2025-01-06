import { Injectable } from "@angular/core";
import { ExtendedCrudServiceImpl } from "../../../shared/request/extended-crud-service-impl";
import { ProcessEventDto } from "../models/process-manager-dto";
import { ProcessEventCrudService } from "./process-event-crud.service";
import { shared } from "../../../shared/shared";

@Injectable({
    providedIn: 'root'
})
export class ProcessEventService extends ExtendedCrudServiceImpl<ProcessEventDto> {
    constructor(private processInstanceCrudService: ProcessEventCrudService) 
    {
        super(processInstanceCrudService);
    }

    getProcessEventsByUserId(processId: number, pageNumber: number, pageSize: number, responseCallback?: (response: shared.response.Object<any>) => void) {
        return this.getWithPaginationTemplate(pageNumber, pageSize, (pageNumber: number, pageSize: number) => {
            return this.processInstanceCrudService.getByProcessId(processId, pageNumber, pageSize);
        }, responseCallback);
    }
}
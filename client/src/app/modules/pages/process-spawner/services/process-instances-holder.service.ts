import { Injectable } from "@angular/core";
import { ProcessInstanceDto } from "../models/process-instance-dto";
import { shared } from "../../../shared/shared";

@Injectable({
    providedIn: 'root'
})
export class ProcessInstancesHolderService extends shared.services.ExtendStateHolder<ProcessInstanceDto> {
    constructor() {
        super();
    }
}
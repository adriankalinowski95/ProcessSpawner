import { Injectable } from "@angular/core";
import { shared } from "../../../shared/shared";
import { ProcessManagerDto } from "../models/process-manager-dto";

@Injectable({
    providedIn: 'root'
})
export class ProcessManagersHolderService extends shared.services.ExtendStateHolder<ProcessManagerDto> {
    constructor() {
        super();
    }
}
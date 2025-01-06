import { Injectable } from "@angular/core";
import { shared } from "../../../shared/shared";
import { ProcessEventDto } from "../models/process-manager-dto";

@Injectable({
    providedIn: 'root'
})
export class ProcessEventsHolderService extends shared.services.ExtendStateHolder<ProcessEventDto> {
    constructor() {
        super();
    }
}
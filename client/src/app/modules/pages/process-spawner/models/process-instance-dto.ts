import { ProcessStatus } from "../enums/process-status.enum";
import { DataSource } from '../../../shared/element-list/models/data-source';

export interface ProcessInstanceDto{
    id: string;
    processType: string;
    parameters: Map<string, string>;
    processId: string;
    status: ProcessStatus;
}
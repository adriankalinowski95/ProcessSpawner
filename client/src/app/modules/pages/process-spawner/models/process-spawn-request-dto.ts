import { shared } from "../../../shared/shared"
import { ProcessType } from "../enums/process-type.enum";

export interface ProcessSpawnRequestDto {
    process_type: ProcessType;
    parameters: Map<string, string>;
}

function isMapOfStringToString(value: unknown): value is Map<string, string> {
    if (!(value instanceof Map) && !(typeof value !== 'object')) {
        return false;
    }

    if (value instanceof Map) {
        for (const [key, val] of value) {
            if (typeof key !== 'string' || typeof val !== 'string') {
                return false;
            }
        }

        return true;
    }

    if (typeof value !== 'object') {
        return true;
    }

    return false;
}

function isProcessTypeEnum(value: unknown): value is ProcessType {
    const result = Object.values(ProcessType);

    return result.includes(value as ProcessType);
  }

export function isProcessSpawnRequestDto(obj: unknown): obj is ProcessSpawnRequestDto {
    if (typeof obj !== 'object' || obj === null || obj === undefined) {
        return false;
    }   
    
    const candidate = obj as Partial<ProcessSpawnRequestDto>; 
    if (!isProcessTypeEnum(candidate.process_type)) {
        return false;
    }
    
    if (!isMapOfStringToString(candidate.parameters)) {
        return false;
    }   

    return true;
}

function parseProcessTypeEnum(value: unknown): ProcessType | undefined {
    if (typeof value === 'number' && Object.values(ProcessType).includes(value as ProcessType)) {
      return value as ProcessType | undefined;
    }

    return undefined;
}

function parseData(plainData: unknown): Map<string, string> | undefined{
    if (typeof plainData !== 'object' || plainData === null) {
        return undefined;
    }   

    const record = plainData as Record<string, unknown>;    
    const entries: [string, string][] = Object.entries(record).map(([k, v]) => {
        if (typeof v !== 'string') {
            throw new Error("Not every field is string");
        }

        return [k, v];
    }); 

    return new Map(entries);
}

export function toProcessSpawnRequestDto(obj: any): ProcessSpawnRequestDto | undefined {
    const type = parseProcessTypeEnum(obj.process_type);
    if (!shared.isNotNullOrUndefined(type)) {
        return undefined;
    }

    return {
        process_type: type,
        parameters: obj.parameters,
    };
}
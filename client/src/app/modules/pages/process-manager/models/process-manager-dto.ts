export interface ProcessManagerDto {
    id: number;
    name: string;
    address: string;
    port: number;
}

export function isProcessManagerDto(obj: unknown): obj is ProcessManagerDto {
    if (typeof obj !== 'object' || obj === null || obj === undefined) {
        return false;
    }   
    
    return 'id' in obj && 'name' in obj && 'address' in obj && 'port' in obj;
}
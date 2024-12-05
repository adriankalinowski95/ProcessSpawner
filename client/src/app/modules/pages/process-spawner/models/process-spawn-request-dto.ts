export interface ProcessSpawnRequestDto {
    process_type: string;
    parameters: Map<string, string>;
}
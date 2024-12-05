export interface SpawnProcessRequestDto {
    process_type: string;
    parameters: Map<string, string>;
}
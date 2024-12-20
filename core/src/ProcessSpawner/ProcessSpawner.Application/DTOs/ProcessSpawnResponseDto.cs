using System;

namespace ProcessSpawner.Application.DTOs {
    public record ProcessSpawnResponseDto(UInt32 process_id, string internal_id, Int64 created_time_ms, Int64 last_update_time_ms, bool success, string message);
}
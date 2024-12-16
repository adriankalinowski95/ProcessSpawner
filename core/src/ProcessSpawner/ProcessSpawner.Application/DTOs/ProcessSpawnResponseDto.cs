using System;

namespace ProcessSpawner.Application.DTOs {
    public record ProcessSpawnResponseDto(UInt32 process_id, string internal_id, bool success, string message);
}
using System;

namespace ProcessSpawner.Application.DTOs {
    public record ProcessSpawnResponseDto(bool success, string message, string process_id);
}
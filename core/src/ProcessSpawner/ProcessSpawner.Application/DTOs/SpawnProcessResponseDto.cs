using System;

namespace ProcessSpawner.Application.DTOs {
    public record SpawnProcessResponseDto(bool success, string message, string process_id);
}
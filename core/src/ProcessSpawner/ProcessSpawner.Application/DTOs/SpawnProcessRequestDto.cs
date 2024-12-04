using System;
namespace ProcessSpawner.Application.DTOs {
    public record SpawnProcessRequestDto(string process_type, Dictionary<string, string> parameters);
}
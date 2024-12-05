using System;
namespace ProcessSpawner.Application.DTOs {
    public record ProcessSpawnRequestDto(string process_type, Dictionary<string, string> parameters);
}
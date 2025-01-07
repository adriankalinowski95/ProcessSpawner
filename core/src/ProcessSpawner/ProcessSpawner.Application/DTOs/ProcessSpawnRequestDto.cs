using System;
namespace ProcessSpawner.Application.DTOs {
    public record ProcessSpawnRequestDto(
        ProcessSpawner.Protobuf.Communication.ProcessType process_type,
        Dictionary<string, string> parameters
    );
}
using System;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Application.Commands {
    public interface IProcessManagerSpawnProcessCommand {
        Task<ProcessSpawnResponseDto> SpawnProcess(ProcessManagerConfig config, DTOs.ProcessSpawnRequestDto request);
    }
}
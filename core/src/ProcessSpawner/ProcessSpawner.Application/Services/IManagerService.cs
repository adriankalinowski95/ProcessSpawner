using System;
using ProcessSpawner.Application.DTOs;

namespace ProcessSpawner.Application.Services {
    public interface IManagerService {
        Task<ProcessSpawnResponseDto> SpawnProcess(DTOs.ProcessSpawnRequestDto startBotRequest);
    }
}
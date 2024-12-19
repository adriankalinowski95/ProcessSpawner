using System;
using ProcessSpawner.Application.DTOs;

namespace ProcessSpawner.Application.Services {
    public interface IProcessManagerSpawningCommunication {
        Task<ProcessSpawnResponseDto> SpawnProcess(DTOs.ProcessSpawnRequestDto startBotRequest);
    }
}
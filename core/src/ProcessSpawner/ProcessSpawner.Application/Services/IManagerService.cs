using System;
using ProcessSpawner.Application.DTOs;

namespace ProcessSpawner.Application.Services {
    public interface IManagerService {
        Task<SpawnProcessResponseDto> SpawnProcess(DTOs.SpawnProcessRequestDto startBotRequest);
    }
}
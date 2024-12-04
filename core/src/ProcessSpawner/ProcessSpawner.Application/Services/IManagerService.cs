using System;
using ProcessSpawner.Application.DTOs;

namespace ProcessSpawner.Application.Services {
    public interface IManagerService {
        Task<Shared.Generic.RestApi.ObjectOperationResult<SpawnProcessResponseDto>> SpawnProcess(DTOs.SpawnProcessRequestDto startBotRequest);
    }
}
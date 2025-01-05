using System;
using ProcessSpawner.Application.DTOs;
using Shared.Generic.RestApi;

namespace ProcessSpawner.Application.Services.REST {
    public interface IProcessSpawningService {
        Task<ObjectOperationResult<ProcessInstanceDto>> SpawnProcess(ProcessSpawnRequestDto obj);
        Task<ObjectOperationResult<ProcessInstanceDto>> FinishProcess(int id);
    }
}
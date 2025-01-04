using System;
using ProcessSpawner.Application.DTOs;
using Shared.Generic.RestApi;
using Shared.Generic.Services.Int;

namespace ProcessSpawner.Application.Services.REST {
    public interface IProcessSpawningService {
        Task<ObjectOperationResult<ProcessInstanceDto>> Get(int id);
        Task<ObjectsResponse<ProcessInstanceDto>> GetAll();
        Task<ObjectOperationResult<ProcessInstanceDto>> Create(ProcessSpawnRequestDto obj);
        Task<ObjectOperationResult<ProcessInstanceDto>> Put(int id, ProcessInstanceDto obj);
        Task<ObjectOperationResult<ProcessInstanceDto>> Delete(int id);
        Task<ObjectOperationResult<ProcessInstanceDto>> FinishProcess(int id);
    }
}
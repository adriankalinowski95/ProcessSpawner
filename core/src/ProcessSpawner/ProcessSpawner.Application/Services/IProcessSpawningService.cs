using System;
using ProcessSpawner.Application.DTOs;
using Shared.Generic.RestApi;
using Shared.Generic.Services.Int;

namespace ProcessSpawner.Application.Services {
    public interface IProcessSpawningService {
        Task<ObjectOperationResult<ProcessInstanceDto>> Get(int id);
        Task<ObjectsResponse<ProcessInstanceDto>> GetAll();
        Task<ObjectOperationResult<ProcessInstanceDto>> Create(SpawnProcessRequestDto obj);
        Task<ObjectOperationResult<ProcessInstanceDto>> Put(int id, ProcessInstanceDto obj);
        Task<ObjectOperationResult<ProcessInstanceDto>> Delete(int id);
    }
}
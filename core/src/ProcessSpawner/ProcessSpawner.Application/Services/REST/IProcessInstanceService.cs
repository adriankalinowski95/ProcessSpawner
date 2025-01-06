using System;
using ProcessSpawner.Application.DTOs;
using Shared.Generic.RestApi;
using Shared.Generic.Services.Int;

namespace ProcessSpawner.Application.Services.REST {
    public interface IProcessInstanceService : ICrudService<ProcessInstanceDto> {
        Task<BasePaginationResponse<ProcessInstanceDto>> GetByUserId(int userId, int pageNumber, int pageSize);
        Task<BasePaginationResponse<ProcessInstanceDto>> GetByManagerId(int managerId, int pageNumber, int pageSize);
    }
}
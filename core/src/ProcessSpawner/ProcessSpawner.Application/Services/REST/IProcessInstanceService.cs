using System;
using ProcessSpawner.Application.DTOs;
using Shared.Generic.RestApi;
using Shared.Generic.Services.Int;
using Shared.Types.Generic.RestApi;

namespace ProcessSpawner.Application.Services.REST {
    public interface IProcessInstanceService : ICrudService<ProcessInstanceDto> {
        Task<BasePaginationConfigResponse> GetPaginationConfigForUser(string userId);
        Task<BasePaginationConfigResponse> GetPaginationConfigForManager(int managerId);
        Task<BasePaginationResponse<ProcessInstanceDto>> GetByUserId(string userId, int pageNumber, int pageSize);
        Task<BasePaginationResponse<ProcessInstanceDto>> GetByManagerId(int managerId, int pageNumber, int pageSize);
    }
}
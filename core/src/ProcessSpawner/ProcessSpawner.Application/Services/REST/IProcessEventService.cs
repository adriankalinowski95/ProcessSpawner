using System;
using ProcessSpawner.Application.DTOs;
using Shared.Generic.RestApi;
using Shared.Generic.Services.Int;
using Shared.Types.Generic.RestApi;

namespace ProcessSpawner.Application.Services.REST {
    public interface IProcessEventService : ICrudService<ProcessEventDto> {
        Task<BasePaginationConfigResponse> GetPaginationConfig(int processInstanceId);
        Task<BasePaginationResponse<ProcessEventDto>> GetByProcessInstanceId(int processInstanceId, int pageNumber, int pageSize);
    }
}
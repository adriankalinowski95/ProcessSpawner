using System;
using ProcessSpawner.Application.DTOs;
using Shared.Generic.RestApi;
using Shared.Generic.Services.Int;

namespace ProcessSpawner.Application.Services.REST {
    public interface IProcessEventService : ICrudService<ProcessEventDto> {
        Task<BasePaginationResponse<ProcessEventDto>> GetByProcessInstanceId(int processInstanceId, int pageNumber, int pageSize);
    }
}
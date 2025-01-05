using System;
using ProcessSpawner.Application.DTOs;
using Shared.Generic.Services.Int;

namespace ProcessSpawner.Application.Services.REST {
    public interface IProcessInstanceService : ICrudService<ProcessInstanceDto> { }
}
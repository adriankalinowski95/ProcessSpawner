using System;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Domain.Models;
using Shared.Generic.Services.Int;

namespace ProcessSpawner.Application.Services.REST {
    public interface IProcessManagerService : ICrudService<ProcessManagerDto> { }
}
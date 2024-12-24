using System;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Application.Commands {
    public interface IProcessManagerFinishProcessCommand {
        Task<FinishProcessResponseDto> FinishProcess(ProcessManagerConfig config, DTOs.FinishProcessRequestDto request);
    }
}
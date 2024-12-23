﻿using System;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Application.Commands {
    public interface IProcessManagerQueryCommand {
        Task<QueryResponseDto> QueryProcesses(ProcessManagerConfig config, DTOs.QueryRequestDto request);
    }
}
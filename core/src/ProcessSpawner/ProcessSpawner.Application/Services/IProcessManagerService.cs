﻿using System;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Application.Services {
    public interface IProcessManagerService {
        Task<ProcessManager> GetAvailableProcessManager();
        // @Todo crud
    }
}
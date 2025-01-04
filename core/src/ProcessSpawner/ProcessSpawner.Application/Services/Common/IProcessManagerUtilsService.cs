using System;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Application.Services.Common {
    public interface IProcessManagerUtilsService {
        Task<ProcessManager> GetAvailableProcessManager();
        // @Todo crud
    }
}
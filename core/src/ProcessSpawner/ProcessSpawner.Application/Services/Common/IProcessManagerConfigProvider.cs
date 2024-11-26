using System;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Application.Services.Common {
    public interface IProcessManagerConfigProvider {
        ProcessManagerConfig GetConfig();
    };
}


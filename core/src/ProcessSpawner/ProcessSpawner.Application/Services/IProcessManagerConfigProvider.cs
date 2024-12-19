using System;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Application.Services {
    public interface IProcessManagerConfigProvider {
        ProcessManagerConfig GetConfig();
    };
}


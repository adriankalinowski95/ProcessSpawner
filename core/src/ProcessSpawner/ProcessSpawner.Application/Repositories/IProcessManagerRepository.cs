using System;
using ProcessSpawner.Domain.Models;
using Shared.Generic.Repositories.Int;

namespace ProcessSpawner.Application.Repositories {
    public interface IProcessManagerRepository : IGenericRepository<Domain.Models.ProcessManager> {
        ProcessManager? GetByName(string name);
    }
}
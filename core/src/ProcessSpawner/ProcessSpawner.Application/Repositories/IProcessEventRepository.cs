using System;
using Shared.Generic.Repositories.Int;

namespace ProcessSpawner.Application.Repositories {
    public interface IProcessEventRepository : IGenericRepository<Domain.Models.ProcessEvent> {
        Task<IEnumerable<Domain.Models.ProcessEvent>> GetByInternalIdAsync(string internalId);
    }
}
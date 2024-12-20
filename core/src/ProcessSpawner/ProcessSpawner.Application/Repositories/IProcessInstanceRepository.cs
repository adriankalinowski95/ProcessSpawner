using System;
using Shared.Generic.Repositories.Int;

namespace ProcessSpawner.Application.Repositories {
    public interface IProcessInstanceRepository : IGenericRepository<Domain.Models.ProcessInstance> {
        Task<Domain.Models.ProcessInstance> GetByInternalIdAsync(string internalId);
        Task<int> DeactiveProccesesForManager(int managerId);
    }
}
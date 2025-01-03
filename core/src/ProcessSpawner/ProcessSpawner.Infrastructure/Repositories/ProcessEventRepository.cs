using System;
using Microsoft.EntityFrameworkCore;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Domain.Models;
using Shared.Types.Db;

namespace ProcessSpawner.Infrastructure.Repositories {
    public class ProcessEventRepository : Shared.Generic.Repositories.Int.GenericRepository<ProcessEvent>, IProcessEventRepository {
        public ProcessEventRepository(DatabaseContext dbContext) : base(dbContext) { }

        public async Task<IEnumerable<Domain.Models.ProcessEvent>> GetByInternalIdAsync(string internalId) {
            return await m_dbSet.Where(process => process.ProcessInstance.InternalId == internalId).ToListAsync();
        }
    }
}


using System;
using Microsoft.EntityFrameworkCore;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Repositories {
    public class ProcessManagerRepository : Shared.Generic.Repositories.Int.GenericRepository<ProcessManager>, IProcessManagerRepository {
        public ProcessManagerRepository(DbContext dbContext) : base(dbContext) { }

        public Task<ProcessManager> GetByNameAsync(string name) {
            return m_dbSet.Where(manager => manager.Name == name).FirstOrDefaultAsync();
        }
    }
}
using System.Collections;
using System.Xml.Linq;
using Authorization.Domain.Models;
using Microsoft.EntityFrameworkCore;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Repositories {
    public class ProcessInstanceRepository : Shared.Generic.Repositories.Int.GenericRepository<ProcessInstance>, IProcessInstanceRepository {
        public ProcessInstanceRepository(DbContext dbContext) : base(dbContext) { }

        public async Task<Domain.Models.ProcessInstance> GetByInternalIdAsync(string internalId) {
            return await m_dbSet.Where(process => process.InternalId == internalId).FirstOrDefaultAsync();
        }

        public async Task<int> DeactiveProccesesForManager(int managerId) {
            var result = await m_dbSet
                .Where(process => process.ProcessManagerId == managerId && process.Status == Domain.Enums.ProcessStatus.Active || process.Status == Domain.Enums.ProcessStatus.Started)
                .ExecuteUpdateAsync(process => process.SetProperty(x => x.Status, Domain.Enums.ProcessStatus.NonActive));

            await m_context.SaveChangesAsync();

            return result;
        }
    }
}
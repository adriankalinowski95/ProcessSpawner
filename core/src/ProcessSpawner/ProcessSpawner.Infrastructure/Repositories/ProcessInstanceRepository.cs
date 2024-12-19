using System.Collections;
using Authorization.Domain.Models;
using Microsoft.EntityFrameworkCore;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Repositories {
    public class ProcessInstanceRepository : Shared.Generic.Repositories.Int.GenericRepository<ProcessInstance>, IProcessInstanceRepository {
        public ProcessInstanceRepository(DbContext dbContext) : base(dbContext) {
            Console.WriteLine("process instance repository");
        }
    }
}
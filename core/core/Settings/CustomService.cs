using System;
using Authorization.Infrastructure.Mapping;
using Microsoft.EntityFrameworkCore;
using ProcessSpawner.Infrastructure.Mapping;
using Shared.Tools.Logging.Mapping;

namespace core.Settings {
    public class CustomService : Shared.Types.Db.ICustomService {
        public void ConfigureModel(ModelBuilder modelBuilder) {
            modelBuilder.ApplyConfigurationsFromAssembly(typeof(UserMap).Assembly);
            modelBuilder.ApplyConfigurationsFromAssembly(typeof(LogMap).Assembly);
            modelBuilder.ApplyConfigurationsFromAssembly(typeof(ProcessInstanceMap).Assembly);
            modelBuilder.ApplyConfigurationsFromAssembly(typeof(ProcessManagerMap).Assembly);
        }
    }
}


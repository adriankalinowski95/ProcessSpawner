using System;
using Authorization.Domain.Models;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Builders;
using Microsoft.Extensions.Configuration;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Mapping {
    public class ProcessManagerMap : IEntityTypeConfiguration<ProcessManager> {
        public void Configure(EntityTypeBuilder<ProcessManager> builder) {
            /*
            builder.HasMany<ProcessInstance>(u => u.ProcessInstances)
                   .WithOne(u => u.ProcessManager)
                   .HasForeignKey(u => u.ProcessManagerId);
            */
            builder.HasIndex(u => u.Name)
                   .IsUnique();

            builder.Navigation(u => u.ProcessInstances)
                  .AutoInclude();

            var isStatic = false;
            if (isStatic) {
                /*
                var defaultProcessManager = new ProcessManager {
                    Id = 435345,
                    Name = "default_process_manager",
                    Address = "http://192.168.1.190",
                    Port = 50051
                };

                builder.HasData(defaultProcessManager);
                */
            } else {
                /*
                var defaultProcessManager = new ProcessManager {
                    Id = 435345,
                    Name = "default_process_manager",
                    Address = "http://localhost",
                    Port = 50051
                };

                builder.HasData(defaultProcessManager);
                */
            }
        }
    }
}
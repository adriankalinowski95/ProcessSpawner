using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Builders;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Mapping {
    public class ProcessManagerMap : IEntityTypeConfiguration<ProcessManager> {
        public void Configure(EntityTypeBuilder<ProcessManager> builder) {
            builder.HasMany<ProcessInstance>(u => u.ProcessInstances)
                   .WithOne(u => u.ProcessManager)
                   .HasForeignKey(u => u.ProcessManagerId);

            builder.Navigation(u => u.ProcessInstances)
                  .AutoInclude();
        }
    }
}
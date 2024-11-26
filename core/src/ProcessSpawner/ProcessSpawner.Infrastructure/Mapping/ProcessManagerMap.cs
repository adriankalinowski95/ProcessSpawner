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
            builder.HasMany<ProcessInstance>(u => u.ProcessInstances)
                   .WithOne(u => u.ProcessManager)
                   .HasForeignKey(u => u.ProcessManagerId)
                   .OnDelete(DeleteBehavior.Cascade);

            builder.HasIndex(u => u.Name)
                   .IsUnique();

            builder.Navigation(u => u.ProcessInstances)
                  .AutoInclude();
        }
    }
}
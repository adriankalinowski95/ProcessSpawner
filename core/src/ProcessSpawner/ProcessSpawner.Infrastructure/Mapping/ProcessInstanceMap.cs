using System;
using System.Reflection.Emit;
using System.Xml;
using Authorization.Domain.Models;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Builders;
using ProcessSpawner.Domain.Models;
using System.Text.Json;

namespace ProcessSpawner.Infrastructure.Mapping {
    public class ProcessInstanceMap : IEntityTypeConfiguration<ProcessInstance> {
        public void Configure(EntityTypeBuilder<ProcessInstance> builder) {
            builder
            .Property(e => e.Parameters)
            .HasConversion(
                v => JsonSerializer.Serialize(v, new JsonSerializerOptions { WriteIndented = false }),
                v => JsonSerializer.Deserialize<Dictionary<string, string>>(v, new JsonSerializerOptions()) ?? new Dictionary<string, string>()
            );
            builder.HasMany<ProcessEvent>(u => u.ProcessEvents)
                   .WithOne(u => u.ProcessInstance)
                   .HasForeignKey(u => u.ProcessInstanceId)
                   .OnDelete(DeleteBehavior.Cascade);

            builder.Navigation(u => u.ProcessManager)
                  .AutoInclude();

            /*
            builder.Navigation(u => u.ProcessEvents)
                  .AutoInclude();
            
            builder.HasOne(u => u.ProcessManager)
                   .WithMany(u => u.ProcessInstances)
                   .HasForeignKey(u => u.ProcessManagerId);
            */

        }
    }
}
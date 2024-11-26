using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Builders;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Mapping {
    public class ProcessEventMap : IEntityTypeConfiguration<ProcessEvent> {
        public void Configure(EntityTypeBuilder<ProcessEvent> builder) {
            //builder.Navigation(u => u.ProcessInstance)
            //      .AutoInclude();
        }
    }
}


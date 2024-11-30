using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Builders;
using Shared.Tools.Logging.Models;

namespace Shared.Tools.Logging.Mapping {
    public class LogMap : IEntityTypeConfiguration<Log> {
        public void Configure(EntityTypeBuilder<Log> builder) {

        }
    }
}
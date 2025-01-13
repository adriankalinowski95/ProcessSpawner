using System;
using Authorization.Domain.Models;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Builders;

namespace Authorization.Infrastructure.Mapping {
    public class RefreshTokenMap : IEntityTypeConfiguration<RefreshToken> {
        public void Configure(EntityTypeBuilder<RefreshToken> builder) {

        }
    }
}


using System;
using Authorization.Domain.Models;
using Authorization.Infrastructure.Services;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Builders;

namespace Authorization.Infrastructure.Mapping {
    public class UserMap : IEntityTypeConfiguration<User> {
        public void Configure(EntityTypeBuilder<User> builder) {
            var hashingService = new HashingService();
            var newUser = new User {
                Id = (int) 986789339,
                Email = "admin@gmail.com",
                Username = "admin@gmail.com",
                Password = hashingService.HashString("admin123456"),
                Lastname = "",
                Firstname = ""
            };

            builder.HasData(newUser);
        }
    }
}
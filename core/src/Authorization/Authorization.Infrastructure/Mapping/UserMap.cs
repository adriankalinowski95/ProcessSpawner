using System;
using Authorization.Domain.Models;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Builders;

namespace Authorization.Infrastructure.Mapping {
    public class UserMap : IEntityTypeConfiguration<User> {
        public void Configure(EntityTypeBuilder<User> builder) {
            var newUser = new User { Id = (int) 986789339, Email = "admin@gmail.com", Username = "admin@gmail.com", Password = "admin123456", Lastname = "", Firstname = "" };
            builder.HasData(newUser);
        }
    }
}
using System;
using System.Reflection;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore;

namespace Shared.Types.Db {
    public class DatabaseContext : IdentityDbContext<Authorization.Domain.Models.User> {
        private readonly ICustomService m_customService;

        public DatabaseContext(DbContextOptions<DatabaseContext> options,
            ICustomService customService)
            : base(options) {
            var folder = Environment.SpecialFolder.LocalApplicationData;
            var path = Environment.GetFolderPath(folder);

            DbPath = System.IO.Path.Join(path, "main2.db");

            m_customService = customService;
        }

        public string DbPath { get; }

        protected override void OnConfiguring(DbContextOptionsBuilder options) {
            options.UseSqlite($"Data Source={DbPath}", b => {
                b.MigrationsAssembly("core");
            });
        }

        protected override void OnModelCreating(ModelBuilder builder) {
            base.OnModelCreating(builder);
            m_customService.ConfigureModel(builder);
        }
    }
}
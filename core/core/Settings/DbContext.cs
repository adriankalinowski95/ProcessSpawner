using System;
using System.Reflection;
using System.Xml;
using Authorization.Domain.Models;
using Authorization.Infrastructure.Mapping;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Internal;
using ProcessSpawner.Infrastructure.Mapping;
using Shared.Tools.Logging.Mapping;

namespace core.Settings {
    public class DatabaseContext : DbContext {
        public DatabaseContext(DbContextOptions<DatabaseContext> options)
            : base(options) {
            var folder = Environment.SpecialFolder.LocalApplicationData;
            var path = Environment.GetFolderPath(folder);

            DbPath = System.IO.Path.Join(path, "main2.db");
            Console.WriteLine("path:" + path);
        }

        public string DbPath { get; }

        /*
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
           => optionsBuilder
            .UseNpgsql("postgres://boaqhege:TZBzysuD71WnGcIJpbXpn_vSiU31K3sz@peanut.db.elephantsql.com/boaqhege");
        */

        protected override void OnConfiguring(DbContextOptionsBuilder options) {
            options.UseSqlite($"Data Source={DbPath}");
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder) {
            modelBuilder.ApplyConfigurationsFromAssembly(typeof(UserMap).Assembly);
            modelBuilder.ApplyConfigurationsFromAssembly(typeof(LogMap).Assembly);
            modelBuilder.ApplyConfigurationsFromAssembly(typeof(ProcessInstanceMap).Assembly);
            modelBuilder.ApplyConfigurationsFromAssembly(typeof(ProcessManagerMap).Assembly);
        }
    }
}
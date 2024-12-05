using System;
using Authorization.Infrastructure.Mapping;
using Microsoft.EntityFrameworkCore;
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

            // options.EnableSensitiveDataLogging();
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder) {
            // Console.WriteLine("OnModelCreating!");

            modelBuilder.ApplyConfigurationsFromAssembly(typeof(UserMap).Assembly);
            modelBuilder.ApplyConfigurationsFromAssembly(typeof(LogMap).Assembly);
            modelBuilder.ApplyConfigurationsFromAssembly(typeof(ProcessInstanceMap).Assembly);
        }
    }
}
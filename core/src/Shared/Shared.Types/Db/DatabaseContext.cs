using System;
using System.Reflection;
using Microsoft.EntityFrameworkCore;

namespace Shared.Types.Db {
    public class DatabaseContext : DbContext {
        private readonly ICustomService _customService;

        public DatabaseContext(DbContextOptions<DatabaseContext> options,
            ICustomService customService)
            : base(options) {
            var folder = Environment.SpecialFolder.LocalApplicationData;
            var path = Environment.GetFolderPath(folder);

            DbPath = System.IO.Path.Join(path, "main2.db");
            Console.WriteLine("path:" + path);

            _customService = customService;
        }

        public string DbPath { get; }

        /*
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
           => optionsBuilder
            .UseNpgsql("postgres://boaqhege:TZBzysuD71WnGcIJpbXpn_vSiU31K3sz@peanut.db.elephantsql.com/boaqhege");
        */

        protected override void OnConfiguring(DbContextOptionsBuilder options) {
            options.UseSqlite($"Data Source={DbPath}", b => {
                b.MigrationsAssembly("core");
            });
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder) {
            var assemblies = Assembly.GetExecutingAssembly();
            _customService.ConfigureModel(modelBuilder);

            //..modelBuilder.ApplyConfigurationsFromAssembly(Assembly.GetExecutingAssembly());
            // modelBuilder.ApplyConfiguration()
            // modelBuilder.ApplyConfigurationsFromAssembly(typeof(UserMap).Assembly);
            // modelBuilder.ApplyConfigurationsFromAssembly(typeof(LogMap).Assembly);
            // modelBuilder.ApplyConfigurationsFromAssembly(typeof(ProcessInstanceMap).Assembly);
            // modelBuilder.ApplyConfigurationsFromAssembly(typeof(ProcessManagerMap).Assembly);
        }
    }
}
using System;
using System.Security.Claims;
using Authorization.Domain.Enums;
using Authorization.Domain.Models;
using Authorization.Infrastructure.Services;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services.Common;
using ProcessSpawner.Domain.Models;

namespace core.Settings {
    public class RuntimeSeed {
        readonly WebApplication m_webApplication;

        public RuntimeSeed(WebApplication webApplication) {
            m_webApplication = webApplication;
        }

        public async void Seed() {
            using (var scope = m_webApplication.Services.CreateScope()) {
                var defaultUserEmail = "admin@gmail.com";
                var defaultUserPassword = "Admin123456!";

                var userManager = scope.ServiceProvider.GetRequiredService<UserManager<User>>();
                var roleManager = scope.ServiceProvider.GetRequiredService<RoleManager<IdentityRole>>();

                var newUser = new User {
                    UserName = defaultUserEmail,
                    Email = defaultUserEmail
                    // PasswordHash = new HashingService().HashString("admin123456")
                };

                var defaultUser = await userManager.FindByEmailAsync(defaultUserEmail);
                if (defaultUser == null) {
                    var newIdentity = await userManager.CreateAsync(newUser, defaultUserPassword);

                    // roleManager.Roles.ToList();
                    var newRole = await roleManager.CreateAsync(new IdentityRole(nameof(Roles.Admin)));
                    var newRole2 = await roleManager.CreateAsync(new IdentityRole(nameof(Roles.User)));

                    // If i want add roles
                    var isAdminRoleExist = await roleManager.RoleExistsAsync(nameof(Roles.Admin));
                    var isUserRoleExist = await roleManager.RoleExistsAsync(nameof(Roles.User));
                    if (isAdminRoleExist && isUserRoleExist) {
                        string[] roles = { nameof(Roles.Admin), nameof(Roles.User) };
                        await userManager.AddToRolesAsync(newUser, roles);

                        // var userRoles = await userManager.GetRolesAsync(newUser);
                    }

                    // Claims
                    var userClaim = new Claim("access", "all");
                    userManager.AddClaimAsync(newUser, userClaim);

                }

                var processManagerRepository = scope.ServiceProvider.GetRequiredService<IProcessManagerRepository>();
                var processManagerConfigProvider = scope.ServiceProvider.GetRequiredService<IProcessManagerConfigProvider>();

                var config = processManagerConfigProvider.GetConfig();
                var processManager = await processManagerRepository.GetByNameAsync("default_process_manager");
                if (processManager != null) {
                    if (!config.address.Equals(processManager.Address) || !config.port.Equals(processManager.Port)) {
                        processManagerRepository.Remove(processManager);
                    } else {
                        return;
                    }
                }

                var defaultProcessManager = new ProcessManager {
                    Name = "default_process_manager",
                    Address = config.address,
                    Port = config.port
                };

                await processManagerRepository.AddAsync(defaultProcessManager);
            }
        }
    }
}

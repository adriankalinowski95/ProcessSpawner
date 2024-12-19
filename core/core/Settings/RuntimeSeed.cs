using System;
using Microsoft.EntityFrameworkCore;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Domain.Models;

namespace core.Settings {
    public class RuntimeSeed {
        readonly WebApplication m_webApplication;

        public RuntimeSeed(WebApplication webApplication) {
            m_webApplication = webApplication;
        }

        public void Seed() {
            using (var scope = m_webApplication.Services.CreateScope()) {
                var processManagerRepository = scope.ServiceProvider.GetRequiredService<IProcessManagerRepository>();
                var processManagerConfigProvider = scope.ServiceProvider.GetRequiredService<IProcessManagerConfigProvider>();

                var config = processManagerConfigProvider.GetConfig();
                var processManager = processManagerRepository.GetByName("default_process_manager");
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

                processManagerRepository.AddAsync(defaultProcessManager);
            }

        }
    }
}

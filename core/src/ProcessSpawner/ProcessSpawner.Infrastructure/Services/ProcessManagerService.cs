using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Services {
    public class ProcessManagerService : IProcessManagerService {
        public readonly IProcessManagerRepository m_processManagerRepository;
        public readonly ILogger<ProcessManagerService> m_logger;

        public ProcessManagerService(IProcessManagerRepository processManagerRepository, ILogger<ProcessManagerService> logger) {
            m_processManagerRepository = processManagerRepository;
            m_logger = logger;
        }

        public async Task<ProcessManager> GetAvailableProcessManager() {
            var processManager = await m_processManagerRepository.GetByNameAsync("default_process_manager");

            return processManager;
        }
    }
}
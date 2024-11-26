using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services.Common;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Services.Common {
    public class ProcessManagerUtilsService : IProcessManagerUtilsService {
        public readonly IProcessManagerRepository m_processManagerRepository;
        public readonly ILogger<ProcessManagerUtilsService> m_logger;

        public ProcessManagerUtilsService(IProcessManagerRepository processManagerRepository, ILogger<ProcessManagerUtilsService> logger) {
            m_processManagerRepository = processManagerRepository;
            m_logger = logger;
        }

        public async Task<ProcessManager> GetAvailableProcessManager() {
            // @Todo this should searching for avilable process mangar(they should have status like online etc.)
            // and pick that, who have the smallest amount of processes
            var processManager = await m_processManagerRepository.GetByNameAsync("default_process_manager");

            return processManager;
        }
    }
}
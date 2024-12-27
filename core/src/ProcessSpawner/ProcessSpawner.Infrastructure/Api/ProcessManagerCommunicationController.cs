using System;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Infrastructure.Services;

namespace ProcessSpawner.Infrastructure.Api {
    public class ProcessManagerCommunicationController {
        readonly ILogger<ProcessManagerCommunicationController> m_logger;
        readonly IProcessManagerCommunicationService m_processManagerCommunicationService;

        public ProcessManagerCommunicationController(IProcessManagerCommunicationService processManagerCommnucationService, ILogger<ProcessManagerCommunicationController> logger) {
            m_processManagerCommunicationService = processManagerCommnucationService;
            m_logger = logger;
        }
    }
}
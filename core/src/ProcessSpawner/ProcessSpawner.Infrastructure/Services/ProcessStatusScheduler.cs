using System;
using Microsoft.Extensions.Logging;
using Quartz;

namespace ProcessSpawner.Infrastructure.Services {
    public class ProcessStatusScheduler : IJob {
        public readonly ILogger<ProcessStatusScheduler> m_logger;

        public ProcessStatusScheduler(ILogger<ProcessStatusScheduler> logger) {
            m_logger = logger;
        }

        public Task Execute(IJobExecutionContext context) {
            m_logger.LogError("JOB STARTED2!");

            return Task.CompletedTask;
        }
    }
}


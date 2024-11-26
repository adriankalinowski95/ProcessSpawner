using System;
using System.Diagnostics;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Commands;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Domain.Enums;
using ProcessSpawner.Domain.Models;
using ProcessSpawner.Infrastructure.Commands;
using ProcessSpawner.Infrastructure.Repositories;
using ProcessSpawner.Protobuf;
using ProcessSpawner.Protobuf.Communication;
using Quartz;

namespace ProcessSpawner.Infrastructure.Services.gRPC {
    public class ProcessStatusScheduler : IJob {
        private static long Offline_Threshold = 1000 * 60 * 5; // 5 min

        public readonly IProcessManagerQueryCommand m_processManagerQueryCommand;
        public IProcessInstanceRepository m_processInstanceRepository;
        public IProcessManagerRepository m_processManagerRepository;
        public readonly ILogger<ProcessStatusScheduler> m_logger;

        public ProcessStatusScheduler(
            IProcessManagerQueryCommand processManagerQueryCommand,
            IProcessInstanceRepository processInstanceRepository,
            IProcessManagerRepository processManagerRepository,
            ILogger<ProcessStatusScheduler> logger) {
            m_processManagerQueryCommand = processManagerQueryCommand;
            m_processInstanceRepository = processInstanceRepository;
            m_processManagerRepository = processManagerRepository;
            m_logger = logger;
        }

        public async Task Execute(IJobExecutionContext context) {
            Action<ProcessInstanceDto> parseProcess = async (process) => {
                var processDb = await m_processInstanceRepository.GetByInternalIdAsync(process.InternalId);
                if (processDb == null) {
                    m_logger.LogError("Process from manager doesn't exist in database");

                    return;
                }

                processDb.ProcessId = process.ProcessId;
                if (process.LastUpdateTimeMs < processDb.LastUpdateTimeMs) {
                    processDb.Status = ProcessStatus.NonActive;
                    m_processInstanceRepository.Update(processDb);

                    m_logger.LogError($"Something went wrong with update time in process id:{process.Id}");

                    return;
                }

                processDb.LastUpdateTimeMs = process.LastUpdateTimeMs;

                // @Todo validate - negative value
                var timeDifference = DateTime.Now.ToUniversalTime() - processDb.LastUpdateTimeMs;
                if (timeDifference.TotalMilliseconds < Offline_Threshold) {
                    processDb.Status = ProcessStatus.Active;
                } else {
                    processDb.Status = ProcessStatus.NonActive;
                }

                m_processInstanceRepository.Update(processDb);
            };

            var managers = await m_processManagerRepository.GetAllAsync();
            foreach (var manager in managers) {
                try {
                    // @Todo make validation!
                    await m_processInstanceRepository.DeactiveProccesesForManager(manager.Id);
                    var processManagerConfig = new ProcessManagerConfig(manager.Address, manager.Port);
                    var queryRequestDto = new QueryRequestDto(ProcessQueryType.All);
                    var processes = await m_processManagerQueryCommand.QueryProcesses(processManagerConfig, queryRequestDto);

                    processes.processes.ForEach(parseProcess);
                } catch (Exception ex) {
                    m_logger.LogError($"Process status scheduling(manager id:{manager.Id}) with error {ex.Message}");
                }
            }
        }
    }
}
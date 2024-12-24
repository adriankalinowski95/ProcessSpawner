using System;
using Google.Protobuf;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Infrastructure.Api;
using ProcessSpawner.Protobuf;

namespace ProcessSpawner.Infrastructure.Services {
    public class ProcessMangerInputService : IProcessManagerInputService {
        readonly ILogger<ProcessMangerInputService> m_logger;
        readonly IProcessInstanceRepository m_processInstanceRepository;
        readonly IProcessManagerRepository m_processManagerRepository;

        public ProcessMangerInputService(ILogger<ProcessMangerInputService> logger, IProcessInstanceRepository processInstanceRepository, IProcessManagerRepository processManagerRepository) {
            m_logger = logger;
            m_processInstanceRepository = processInstanceRepository;
            m_processManagerRepository = processManagerRepository;
        }

        public async Task<ProcessManagerInputResponse> GetInput(ProcessManagerInputRequest request) {
            var processManager = await m_processManagerRepository.GetByNameAsync(request.ManagerName);
            if (processManager == null) {
                return fail(request.ManagerName);
            }

            var response = new ProcessManagerInputResponse();
            response.Success = true;
            response.Processes.AddRange(processManager.ProcessInstances
                .Where(process => process.Status == Domain.Enums.ProcessStatus.NonActive || process.Status == Domain.Enums.ProcessStatus.Active || process.Status == Domain.Enums.ProcessStatus.Started)
                .Select(process => {
                    var processInstanceProto = new ProcessSpawner.Protobuf.ProcessInstance {
                        ProcessId = process.ProcessId,
                        InternalId = process.InternalId,
                        ProcessType = process.ProcessType,
                        // @Todo is this works?
                        CreatedTimeMs = process.CreatedTimeMs.Millisecond,
                        LastUpdateTimeMs = process.LastUpdateTimeMs.Millisecond
                    };

                    foreach (var kvp in process.Parameters) {
                        processInstanceProto.Parameters.Add(kvp.Key, kvp.Value);
                    }

                    return processInstanceProto;
                }));

            return response;
        }

        public ProcessManagerInputResponse fail(string processManagerName) {
            var message = $"Can't find process manager for name{processManagerName}";
            m_logger.LogError(message);

            var response = new ProcessManagerInputResponse();
            response.Message = message;
            response.Success = false;

            return response;
        }
    }
}
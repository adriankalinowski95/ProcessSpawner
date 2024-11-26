using System;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services.Example;
using ProcessSpawner.Domain.Models;
using ProcessSpawner.Protobuf.CoreCommunication;

namespace ProcessSpawner.Infrastructure.Services.Example {
    // Its example code for sending a command from process manager to core!
    public class CoreCommandCommunicationService : ICoreCommandCommunicationService {
        readonly ILogger<CoreCommandCommunicationService> m_logger;
        readonly IProcessInstanceRepository m_processInstanceRepository;
        readonly IProcessEventRepository m_processEventRepository;

        public CoreCommandCommunicationService(ILogger<CoreCommandCommunicationService> logger, IProcessInstanceRepository processInstanceRepository, IProcessEventRepository processEventRepository) {
            m_logger = logger;
            m_processInstanceRepository = processInstanceRepository;
            m_processEventRepository = processEventRepository;
        }

        public async Task<CoreCommandResponse> Handle(CoreCommandRequest request) {
            var process = await m_processInstanceRepository.GetByInternalIdAsync(request.Process.InternalId);
            if (process == null) {
                return fail(request.Process.InternalId);
            }

            var newEvent = new ProcessEvent {
                Name = request.EventName,
                Value = request.EventValue,
                CreatedTimeMs = DateTime.Now.ToUniversalTime(),
                ProcessInstanceId = process.Id
            };

            var result = m_processEventRepository.AddAsync(newEvent);
            if (result == null) {
                return fail(request.Process.InternalId);
            }

            var response = new CoreCommandResponse();
            response.Result = new Protobuf.Common.Result();
            response.Result.Message = "Ok";
            response.Result.Success = true;

            return response;
        }

        public CoreCommandResponse fail(string processInternalid) {
            var message = $"Can't find process for internalId {processInternalid}";
            m_logger.LogError(message);

            var response = new CoreCommandResponse();
            response.Result.Message = message;
            response.Result.Success = false;

            return response;
        }
    }
}


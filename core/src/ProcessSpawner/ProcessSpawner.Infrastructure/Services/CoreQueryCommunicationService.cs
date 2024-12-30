using System;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Domain.Models;
using ProcessSpawner.Infrastructure.Repositories;
using ProcessSpawner.Protobuf.CoreCommunication;

namespace ProcessSpawner.Infrastructure.Services {
    public class CoreQueryCommunicationService : ICoreQueryCommunicationService {
        readonly ILogger<CoreQueryCommunicationService> m_logger;
        readonly IProcessInstanceRepository m_processInstanceRepository;

        public CoreQueryCommunicationService(ILogger<CoreQueryCommunicationService> logger, IProcessInstanceRepository processInstanceRepository) {
            m_logger = logger;
            m_processInstanceRepository = processInstanceRepository;
        }

        public async Task<Protobuf.CoreCommunication.CoreQueryResponse> Handle(Protobuf.CoreCommunication.CoreQueryRequest request) {
            var process = await m_processInstanceRepository.GetByInternalIdAsync(request.Process.InternalId);
            if (process == null) {
                return fail(request.Process.InternalId);
            }

            var response = new CoreQueryResponse();
            response.ParamValue = "paramValue1";
            response.Result = new Protobuf.Common.Result();
            response.Result.Message = "Ok";
            response.Result.Success = true;

            return response;
        }

        public CoreQueryResponse fail(string processInternalid) {
            var message = $"Can't find process for internalId {processInternalid}";
            m_logger.LogError(message);

            var response = new CoreQueryResponse();
            response.Result.Message = message;
            response.Result.Success = false;

            return response;
        }
    }
}


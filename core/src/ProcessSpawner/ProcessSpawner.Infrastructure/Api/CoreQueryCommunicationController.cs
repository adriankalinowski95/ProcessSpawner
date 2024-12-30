using System;
using Grpc.Core;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Services;

namespace ProcessSpawner.Infrastructure.Api {
    public class CoreQueryCommunicationController : ProcessSpawner.Protobuf.CoreCommunication.CoreQueryCommunicationService.CoreQueryCommunicationServiceBase {
        readonly ILogger<CoreQueryCommunicationController> m_logger;
        readonly ICoreQueryCommunicationService m_service;

        public CoreQueryCommunicationController(ILogger<CoreQueryCommunicationController> logger, ICoreQueryCommunicationService service) {
            m_logger = logger;
            m_service = service;
        }

        public override Task<ProcessSpawner.Protobuf.CoreCommunication.CoreQueryResponse> Handle(global::ProcessSpawner.Protobuf.CoreCommunication.CoreQueryRequest request, ServerCallContext context) {
            return m_service.Handle(request);
        }
    }
}
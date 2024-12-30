using System;
using Grpc.Core;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Services;

namespace ProcessSpawner.Infrastructure.Api {
    public class CoreCommandCommunicationController : ProcessSpawner.Protobuf.CoreCommunication.CoreCommandCommunicationService.CoreCommandCommunicationServiceBase {
        readonly ILogger<CoreCommandCommunicationController> m_logger;
        readonly ICoreCommandCommunicationService m_service;

        public CoreCommandCommunicationController(ILogger<CoreCommandCommunicationController> logger, ICoreCommandCommunicationService service) {
            m_logger = logger;
            m_service = service;
        }

        public override Task<ProcessSpawner.Protobuf.CoreCommunication.CoreCommandResponse> Handle(global::ProcessSpawner.Protobuf.CoreCommunication.CoreCommandRequest request, ServerCallContext context) {
            return m_service.Handle(request);
        }

        //public override Task<ProcessSpawner.Protobuf.CoreCommunication.CoreCommandResponse> Handle(global::ProcessSpawner.Protobuf.CoreCommunication.CoreCommandRequest request, ServerCallContext context) {
        //    throw new RpcException(new Status(StatusCode.Unimplemented, ""));
        //}
    }
}
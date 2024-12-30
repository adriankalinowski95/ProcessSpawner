using System;
using Grpc.Core;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Services;

namespace ProcessSpawner.Infrastructure.Api {
    public class ProcessManagerInputController : ProcessSpawner.Protobuf.Communication.ProcessManagerInputService.ProcessManagerInputServiceBase {
        readonly ILogger<ProcessManagerInputController> m_logger;
        readonly IProcessManagerInputService m_inputService;

        public ProcessManagerInputController(ILogger<ProcessManagerInputController> logger, IProcessManagerInputService inputService) {
            m_logger = logger;
            m_inputService = inputService;
        }

        public override Task<ProcessSpawner.Protobuf.Communication.ProcessManagerInputResponse> GetInput(global::ProcessSpawner.Protobuf.Communication.ProcessManagerInputRequest request, ServerCallContext context) {
            return m_inputService.GetInput(request);
        }
    }
}
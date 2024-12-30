using System;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Commands;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Commands {
    public class ProcessManagerFinishProcessCommand : IProcessManagerFinishProcessCommand {
        public readonly ILogger<ProcessManagerFinishProcessCommand> m_logger;

        public ProcessManagerFinishProcessCommand(ILogger<ProcessManagerFinishProcessCommand> logger) {
            m_logger = logger;
        }

        public async Task<FinishProcessResponseDto> FinishProcess(ProcessManagerConfig config, FinishProcessRequestDto request) {
            var endpoint = config.GetEndpoint();
            if (endpoint == null || endpoint.Length == 0) {
                throw new Exception("Can't find bot manager ip");
            }

            var channel = Grpc.Net.Client.GrpcChannel.ForAddress(endpoint);
            var client = new Protobuf.Communication.SpawnProcessService.SpawnProcessServiceClient(channel);
            var response = await client.FinishProcessAsync(GetRequestProto(request));

            return GetResponseDto(response);
        }

        private Protobuf.Communication.FinishRequest GetRequestProto(FinishProcessRequestDto request) {
            return new Protobuf.Communication.FinishRequest {
                InternalId = request.internal_id
            };
        }

        private FinishProcessResponseDto GetResponseDto(Protobuf.Communication.FinishResponse response) {
            return new FinishProcessResponseDto(
                response.Success,
                response.Message
            );
        }
    }
}
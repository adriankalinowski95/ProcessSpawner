using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Services;
using Shared.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Services {
    public class ProcessManagerSpawningCommunication : IProcessManagerSpawningCommunication {
        public readonly IProcessManagerConfigProvider m_processManagerConfigProvider;
        public readonly ILogger<ProcessManagerSpawningCommunication> m_logger;

        public ProcessManagerSpawningCommunication(IProcessManagerConfigProvider processManagerConfigProvider, ILogger<ProcessManagerSpawningCommunication> logger) {
            m_processManagerConfigProvider = processManagerConfigProvider;
            m_logger = logger;
        }

        public async Task<ProcessSpawnResponseDto> SpawnProcess(ProcessSpawnRequestDto startBotRequest) {
            var endpoint = m_processManagerConfigProvider.GetConfig().GetEndpoint();
            if (endpoint == null || endpoint.Length == 0) {
                throw new Exception("Can't find bot manager ip");
            }

            var channel = Grpc.Net.Client.GrpcChannel.ForAddress(endpoint);
            var client = new Protobuf.ManagerService.ManagerServiceClient(channel);
            var response = await client.SpawnProcessAsync(GetStartRequestProto(startBotRequest));

            return GetSpawnResponseDto(response);
        }

        private Protobuf.SpawnRequest GetStartRequestProto(ProcessSpawnRequestDto request) {
            return new Protobuf.SpawnRequest {
                InternalId = Guid.NewGuid().ToString(),
                ProcessType = request.process_type,
                Parameters = {
                    new Google.Protobuf.Collections.MapField<string, string> { request.parameters }
                },
            };
        }

        private ProcessSpawnResponseDto GetSpawnResponseDto(Protobuf.SpawnResponse response) {
            return new ProcessSpawnResponseDto(
                response.ProcessId,
                response.InternalId,
                response.Success,
                response.Message
            );
        }
    }
}
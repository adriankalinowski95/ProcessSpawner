using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Commands;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Commands {
    public class ProcessManagerSpawnProcessCommand : IProcessManagerSpawnProcessCommand {
        public readonly ILogger<ProcessManagerSpawnProcessCommand> m_logger;

        public ProcessManagerSpawnProcessCommand(ILogger<ProcessManagerSpawnProcessCommand> logger) {
            m_logger = logger;
        }

        public async Task<ProcessSpawnResponseDto> SpawnProcess(ProcessManagerConfig config, ProcessSpawnRequestDto request) {
            var endpoint = config.GetEndpoint();
            if (endpoint == null || endpoint.Length == 0) {
                throw new Exception("Can't find bot manager ip");
            }

            var channel = Grpc.Net.Client.GrpcChannel.ForAddress(endpoint);
            var client = new Protobuf.SpawnProcessService.SpawnProcessServiceClient(channel);
            var response = await client.SpawnProcessAsync(GetRequestProto(request));

            return GetResponseDto(response);
        }

        private Protobuf.SpawnRequest GetRequestProto(ProcessSpawnRequestDto request) {
            return new Protobuf.SpawnRequest {
                InternalId = Guid.NewGuid().ToString(),
                ProcessType = request.process_type,
                Parameters = {
                    new Google.Protobuf.Collections.MapField<string, string> { request.parameters }
                },
            };
        }

        private ProcessSpawnResponseDto GetResponseDto(Protobuf.SpawnResponse response) {
            return new ProcessSpawnResponseDto(
                response.Process.ProcessId,
                response.Process.InternalId,
                response.Process.CreatedTimeMs,
                response.Process.LastUpdateTimeMs,
                response.Success,
                response.Message
            );
        }
    }
}
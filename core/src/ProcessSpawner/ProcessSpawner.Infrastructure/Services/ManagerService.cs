using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Services;
using Shared.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Services {
    public class ManagerService : IManagerService {
        public readonly IConfiguration m_configuration;
        public readonly ILogger<ManagerService> m_logger;

        public ManagerService(IConfiguration configuration, ILogger<ManagerService> logger) {
            m_configuration = configuration;
            m_logger = logger;
        }

        public async Task<ObjectOperationResult<SpawnProcessResponseDto>> SpawnProcess(SpawnProcessRequestDto startBotRequest) {
            var channelIp = m_configuration["gRPCBotManagerIp"];
            if (channelIp == null || channelIp.Length == 0) {
                throw new Exception("Can't find bot manager ip");
            }

            var channel = Grpc.Net.Client.GrpcChannel.ForAddress(channelIp);
            var client = new Protobuf.ManagerService.ManagerServiceClient(channel);

            var response = await client.SpawnProcessAsync(await GetStartRequestProto(startBotRequest));

            return new ObjectOperationResult<SpawnProcessResponseDto> {
                Status = BaseResponseStatus.Ok
            };
        }

        private Task<Protobuf.SpawnRequest> GetStartRequestProto(SpawnProcessRequestDto request) {
            var spawnRequest = new Protobuf.SpawnRequest {
                ProcessType = request.process_type,
                Parameters = { new Google.Protobuf.Collections.MapField<string, string> { request.parameters } }
            };

            return Task.FromResult(spawnRequest);
        }
    }
}
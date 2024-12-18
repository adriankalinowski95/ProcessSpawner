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

        public async Task<ProcessSpawnResponseDto> SpawnProcess(ProcessSpawnRequestDto startBotRequest) {
            var channelIp = GetgRPCManagerAddress();
            if (channelIp == null || channelIp.Length == 0) {
                throw new Exception("Can't find bot manager ip");
            }

            var channel = Grpc.Net.Client.GrpcChannel.ForAddress(channelIp);
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

        private string GetgRPCManagerAddress() {
            var isGRPCManagerStaticAddressVerbExist = Boolean.TryParse(m_configuration["IsgRPCManagerStaticAddress"], out bool isGRPCManagerStaticAddress);

            var address = "";
            if (isGRPCManagerStaticAddressVerbExist && isGRPCManagerStaticAddress) {
                address = m_configuration["StaticgRPCManagerAddress"];
            } else {
                address = m_configuration["gRPCManagerAddress"];
            }

            return address;
        }
    }
}
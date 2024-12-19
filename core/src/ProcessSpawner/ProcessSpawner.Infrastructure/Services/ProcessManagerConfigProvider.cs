using System;
using Microsoft.Extensions.Configuration;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Services {
    public class ProcessManagerConfigProvider : IProcessManagerConfigProvider {
        public readonly IConfiguration m_configuration;

        public ProcessManagerConfigProvider(IConfiguration configuration) {
            m_configuration = configuration;
        }

        public ProcessManagerConfig GetConfig() {
            var isGRPCManagerStaticAddressVerbExist = Boolean.TryParse(m_configuration["IsgRPCManagerStaticAddress"], out bool isGRPCManagerStaticAddress);

            if (isGRPCManagerStaticAddressVerbExist && isGRPCManagerStaticAddress) {
                var gRPCManagerAddress = m_configuration.GetValue<string>("StaticgRPCManagerAddress");
                var gRPCManagerPort = m_configuration.GetValue<uint>("StaticgRPCManagerPort");
                if (gRPCManagerAddress == null || gRPCManagerPort == 0) {
                    throw new KeyNotFoundException("gRPC manager address doesn't exist!");
                }

                return new ProcessManagerConfig(
                    gRPCManagerAddress,
                    gRPCManagerPort
                );
            } else {
                var gRPCManagerAddress = m_configuration.GetValue<string>("gRPCManagerAddress");
                var gRPCManagerPort = m_configuration.GetValue<uint>("gRPCManagerPort");
                if (gRPCManagerAddress == null || gRPCManagerPort == 0) {
                    throw new KeyNotFoundException("gRPC manager address doesn't exist!");
                }

                return new ProcessManagerConfig(
                    gRPCManagerAddress,
                    gRPCManagerPort
                );
            }
        }
    }
}
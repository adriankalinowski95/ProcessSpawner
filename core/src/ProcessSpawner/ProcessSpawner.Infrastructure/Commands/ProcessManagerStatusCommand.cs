using System;
using System.Diagnostics;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Commands;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Infrastructure.Commands {
    public class ProcessManagerStatusCommand : IProcessManagerStatusCommand {
        public readonly ILogger<ProcessManagerStatusCommand> m_logger;

        public ProcessManagerStatusCommand(ILogger<ProcessManagerStatusCommand> logger) {
            m_logger = logger;
        }

        public async Task<QueryResponseDto> QueryProcesses(ProcessManagerConfig config, QueryRequestDto request) {
            var endpoint = config.GetEndpoint();
            if (endpoint == null || endpoint.Length == 0) {
                throw new Exception("Can't find bot manager ip");
            }

            var channel = Grpc.Net.Client.GrpcChannel.ForAddress(endpoint);
            var client = new Protobuf.ProcessQueryService.ProcessQueryServiceClient(channel);
            var response = await client.QueryProcessesAsync(GetRequestProto(request));

            return GetResponseDto(response);
        }

        private Protobuf.QueryRequest GetRequestProto(QueryRequestDto request) {
            return new Protobuf.QueryRequest {
                QueryType = request.query_type
            };
        }

        private QueryResponseDto GetResponseDto(Protobuf.QueryResponse response) {
            var processes = response.Processes.Select(x => {
                return new ProcessInstanceDto {
                    ProcessId = x.ProcessId,
                    InternalId = x.InternalId,
                    CreatedTimeMs = DateTimeOffset.FromUnixTimeMilliseconds(x.CreatedTimeMs).UtcDateTime,
                    LastUpdateTimeMs = DateTimeOffset.FromUnixTimeMilliseconds(x.LastUpdateTimeMs).UtcDateTime
                };
            }).ToList();

            return new QueryResponseDto(
                processes
            );
        }
    }
}
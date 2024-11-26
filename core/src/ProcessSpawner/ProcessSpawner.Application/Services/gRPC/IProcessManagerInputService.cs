using System;
namespace ProcessSpawner.Application.Services.gRPC {
    public interface IProcessManagerInputService {
        Task<ProcessSpawner.Protobuf.Communication.ProcessManagerInputResponse> GetInput(Protobuf.Communication.ProcessManagerInputRequest request);
    }
}
using System;
namespace ProcessSpawner.Application.Services {
    public interface IProcessManagerInputService {
        Task<ProcessSpawner.Protobuf.Communication.ProcessManagerInputResponse> GetInput(Protobuf.Communication.ProcessManagerInputRequest request);
    }
}
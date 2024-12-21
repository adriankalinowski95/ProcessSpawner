using System;
namespace ProcessSpawner.Application.Services {
    public interface IProcessManagerInputService {
        Task<ProcessSpawner.Protobuf.ProcessManagerInputResponse> GetInput(Protobuf.ProcessManagerInputRequest request);
    }
}
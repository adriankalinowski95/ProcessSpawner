using System;
namespace ProcessSpawner.Application.Services.Example {
    public interface ICoreCommandCommunicationService {
        Task<ProcessSpawner.Protobuf.CoreCommunication.CoreCommandResponse> Handle(Protobuf.CoreCommunication.CoreCommandRequest request);
    }
}
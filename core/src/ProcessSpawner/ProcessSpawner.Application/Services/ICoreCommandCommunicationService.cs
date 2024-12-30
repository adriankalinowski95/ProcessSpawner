using System;
namespace ProcessSpawner.Application.Services {
    public interface ICoreCommandCommunicationService {
        Task<ProcessSpawner.Protobuf.CoreCommunication.CoreCommandResponse> Handle(Protobuf.CoreCommunication.CoreCommandRequest request);
    }
}
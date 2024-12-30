using System;
namespace ProcessSpawner.Application.Services {
    public interface ICoreQueryCommunicationService {
        Task<ProcessSpawner.Protobuf.CoreCommunication.CoreQueryResponse> Handle(Protobuf.CoreCommunication.CoreQueryRequest request);
    }
}

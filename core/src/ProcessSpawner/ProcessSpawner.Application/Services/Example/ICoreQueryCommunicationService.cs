using System;
namespace ProcessSpawner.Application.Services.Example {
    public interface ICoreQueryCommunicationService {
        Task<ProcessSpawner.Protobuf.CoreCommunication.CoreQueryResponse> Handle(Protobuf.CoreCommunication.CoreQueryRequest request);
    }
}

using System;

namespace ProcessSpawner.Application.DTOs {
    public record QueryRequestDto(Protobuf.Communication.ProcessQueryType query_type);
}
using System;
using ProcessSpawner.Protobuf;

namespace ProcessSpawner.Application.DTOs {
    public record QueryRequestDto(ProcessQueryType query_type);
}
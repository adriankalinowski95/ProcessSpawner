using System;
namespace ProcessSpawner.Application.DTOs {
    public record ProcessEventDto(int Id, string Name, string Value, DateTime CreatedTimeMs);
}
using System;
namespace ProcessSpawner.Application.DTOs {
    public record ProcessManagerDto(int Id, string Name, string Address, UInt32 Port);
}
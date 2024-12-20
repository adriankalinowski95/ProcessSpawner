using System;
using ProcessSpawner.Domain.Models;

namespace ProcessSpawner.Application.DTOs {
    public record QueryResponseDto(List<ProcessInstanceDto> processes);
}
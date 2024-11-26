using System;

namespace ProcessSpawner.Application.DTOs {
    public record FinishProcessResponseDto(bool success, string message);
}
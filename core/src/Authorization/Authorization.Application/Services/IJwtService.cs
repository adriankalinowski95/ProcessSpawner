using System;
using Authorization.Application.DTOs;
using Authorization.Domain.Models;
using Microsoft.AspNetCore.Identity;

namespace Authorization.Application.Services {
    public interface IJwtService {
        Task<TokenDto> GenerateToken(User user);
        Task<bool> VerifyToken(TokenDto token);
    }
}

using System;
using Authorization.Application.DTOs;
using Authorization.Domain.Models;
using Microsoft.AspNetCore.Identity;

namespace Authorization.Application.Services {
    public interface IAuthManagementService {
        public Task<TokenDto> Login(AuthenticationDto authentication);
        public Task<User> GetCurrentUser();
        public Task<UserDto> GetCurrentUserDto();
        public Task<TokenDto> RefreshToken(TokenDto token);
    }
}
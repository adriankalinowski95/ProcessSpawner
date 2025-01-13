using System;
using System.Security.Claims;
using Authorization.Application.DTOs;
using Authorization.Application.Repositories;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using Authorization.Infrastructure.Repositories;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Identity;
using Microsoft.IdentityModel.Tokens;

namespace Authorization.Infrastructure.Services {
    public class AuthManagementService : IAuthManagementService {
        private readonly IJwtService m_jwtService;
        private readonly IHttpContextAccessor m_httpContextAccessor;
        private readonly UserManager<User> m_userManager;
        private readonly IRefreshTokenRepository m_refreshTokenRepository;

        public AuthManagementService(
                IJwtService jwtService,
                IHttpContextAccessor httpContextAccessor,
                UserManager<User> userManager,
                IRefreshTokenRepository refreshTokenRepository) {
            m_jwtService = jwtService;
            m_httpContextAccessor = httpContextAccessor;
            m_userManager = userManager;
            m_refreshTokenRepository = refreshTokenRepository;
        }

        public async Task<TokenDto> Login(AuthenticationDto authentication) {
            var user = await m_userManager.FindByEmailAsync(authentication.Username);
            if (user == null) {
                throw new KeyNotFoundException("Can't found user by userName");
            }

            if (!await m_userManager.CheckPasswordAsync(user, authentication.Password)) {
                throw new KeyNotFoundException($"Password for user name: {authentication.Username} is wrong!");
            }

            return await m_jwtService.GenerateToken(user);
        }

        public async Task<User> GetCurrentUser() {
            if (m_httpContextAccessor.HttpContext == null) {
                throw new BadHttpRequestException("http context doesn't exist!");
            }

            var userClaims = m_httpContextAccessor.HttpContext.User;
            if (userClaims == null || userClaims.Identity == null || !userClaims.Identity.IsAuthenticated) {
                throw new UnauthorizedAccessException("User is not authenticated.");
            }

            var userToken = userClaims.FindFirst(ClaimTypes.Email);
            if (userToken == null) {
                throw new KeyNotFoundException("Claims doesn't user type!");
            }

            var user = await m_userManager.FindByEmailAsync(userToken.Value);
            if (user == null) {
                throw new KeyNotFoundException("User doesn't exist!");
            }

            return user;
        }

        public async Task<UserDto> GetCurrentUserDto() {
            var user = await GetCurrentUser();

            return new UserDto {
                Id = user.Id,
                Email = user.Email
            };
        }

        public async Task<TokenDto> RefreshToken(TokenDto token) {
            var isTokenCorrect = await m_jwtService.VerifyToken(token);
            if (!isTokenCorrect) {
                throw new Exception("Can't verify token!");
            }

            var refreshTokenDb = await m_refreshTokenRepository.Get(x => x.Token == token.RefreshToken);
            if (refreshTokenDb == null) {
                throw new Exception("Can't find refresh token!");
            }

            refreshTokenDb.IsUsed = true;
            m_refreshTokenRepository.Update(refreshTokenDb);

            var user = await m_userManager.FindByIdAsync(refreshTokenDb.UserId);
            if (user == null) {
                throw new Exception("Can't find user for token!");
            }

            return await m_jwtService.GenerateToken(user);
        }
    }
}


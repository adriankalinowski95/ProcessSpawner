using System.Security.Claims;
using Authorization.Application.DTOs;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using JWT.Algorithms;
using JWT.Builder;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Configuration;

namespace Authorization.Infrastructure.Services {
    public class UserAuthenticationService : IUserAuthenticationService {
        private readonly IUserRepository m_userRepository;
        private readonly IHashingService m_hashingService;
        private readonly IHttpContextAccessor m_httpContextAccessor;
        private readonly IJwtTokenService m_jwtTokenService;
        private readonly IConfiguration m_config;

        public UserAuthenticationService(
                IUserRepository userRepository,
                IHashingService hashingService,
                IHttpContextAccessor httpContextAccessor,
                IJwtTokenService jwtTokenService,
                IConfiguration config) {
            m_userRepository = userRepository;
            m_hashingService = hashingService;
            m_httpContextAccessor = httpContextAccessor;
            m_jwtTokenService = jwtTokenService;
            m_config = config;
        }

        public User GetCurrentUser() {
            if (m_httpContextAccessor.HttpContext == null) {
                throw new UnauthorizedAccessException("http context doesn't exist!");
            }

            var userClaims = m_httpContextAccessor.HttpContext.User;
            if (userClaims == null || userClaims.Identity == null || !userClaims.Identity.IsAuthenticated) {
                throw new UnauthorizedAccessException("User is not authenticated.");
            }

            var userToken = userClaims.Claims.FirstOrDefault(x => x.Type == "username");
            if (userToken == null) {
                throw new UnauthorizedAccessException("Claims doesn't user type!");
            }

            var user = m_userRepository.GetByName(userToken.Value);
            if (user == null) {
                throw new UnauthorizedAccessException("User doesn't exist!");
            }

            return user;
        }

        public UserDto GetCurrentUserDto() {
            if (m_httpContextAccessor.HttpContext == null) {
                throw new UnauthorizedAccessException("http context doesn't exist!");
            }

            var userClaims = m_httpContextAccessor.HttpContext.User;
            if (userClaims == null || userClaims.Identity == null || !userClaims.Identity.IsAuthenticated) {
                throw new UnauthorizedAccessException("User is not authenticated.");
            }

            var userToken = userClaims.Claims.FirstOrDefault(x => x.Type == "username");
            if (userToken == null) {
                throw new UnauthorizedAccessException("Claims doesn't user type!");
            }

            var user = m_userRepository.GetByName(userToken.Value);
            if (user == null) {
                throw new UnauthorizedAccessException("User doesn't exist!");
            }

            return new UserDto {

            };
        }

        public TokenDto Login(AuthenticationDto authentication) {
            User user = m_userRepository.GetByName(authentication.Username);
            if (user == null) {
                throw new UnauthorizedAccessException("Username or password incorrect");
            }

            var validPassword = m_hashingService.CompareStringWithHash(authentication.Password, user.Password);
            if (!validPassword) {
                throw new UnauthorizedAccessException("Username or password incorrect");
            }

            var refreshToken = m_jwtTokenService.generateRefreshToken(user);

            // Update(user.Id, user);

            // @Todo move this shit to jwttokenservice
            var jwtKey = m_config.GetSection("Jwt:Key").Get<string>();
            var jwtIssuer = m_config.GetSection("Jwt:Issuer").Get<string>();

            var accToken = new JwtBuilder()
                    .WithAlgorithm(new HMACSHA256Algorithm())
                    .WithSecret(jwtKey)
                    .AddClaim("exp", DateTimeOffset.UtcNow.AddHours(1).ToUnixTimeSeconds())
                    .AddClaim("iss", jwtIssuer)
                    .AddClaim("username", user.Username)
                    .Encode();

            return new TokenDto {
                AccessToken = accToken,
                RefreshToken = refreshToken.jwt
            };
        }

        public TokenDto RefreshToken(Claim userClaim, Claim refreshClaim) {
            User user = m_userRepository.GetByName(userClaim.Value);
            if (user == null) {
                throw new System.Exception("User doesn't exist");
            }

            return new TokenDto {
                AccessToken = m_jwtTokenService.generateAccessToken(user),
                RefreshToken = ""
            };
        }
    }
}
using System;
using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;
using System.Text;
using Authorization.Application.DTOs;
using Authorization.Application.Repositories;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using Authorization.Infrastructure.Configuration;
using JWT;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore.Metadata.Internal;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Options;
using Microsoft.IdentityModel.Tokens;
using Shared.Tools.Utils;

namespace Authorization.Infrastructure.Services {
    public class JwtService : IJwtService {
        private static int Access_Token_Expire_Min = 5;

        private readonly JwtConfig m_jwtConfig;
        private readonly IRefreshTokenRepository m_refreshTokenRepository;
        private readonly UserManager<User> m_userManager;
        private readonly RoleManager<IdentityRole> m_roleManager;

        public JwtService(
                IOptionsMonitor<JwtConfig> optionsMonitor,
                IRefreshTokenRepository refreshTokenRepository,
                UserManager<User> userManager,
                RoleManager<IdentityRole> roleManager) {
            m_jwtConfig = optionsMonitor.CurrentValue;
            m_refreshTokenRepository = refreshTokenRepository;
            m_userManager = userManager;
            m_roleManager = roleManager;
        }

        public async Task<TokenDto> GenerateToken(User user) {
            var jwtTokenHandler = new JwtSecurityTokenHandler();
            var key = GeyKeyBytes();
            var tokenDescriptor = new SecurityTokenDescriptor {
                Subject = new ClaimsIdentity(await GetAllValidClaims(user)),
                Expires = DateTime.UtcNow.AddMinutes(Access_Token_Expire_Min),
                SigningCredentials = new SigningCredentials(new SymmetricSecurityKey(key), SecurityAlgorithms.HmacSha256Signature)
            };

            var token = jwtTokenHandler.CreateToken(tokenDescriptor);
            var jwtToken = jwtTokenHandler.WriteToken(token);

            var refreshToken = new RefreshToken {
                JwtId = token.Id,
                IsUsed = false,
                IsRevoked = false,
                UserId = user.Id,
                AddedDate = DateTime.UtcNow,
                ExpiryDate = DateTime.UtcNow.AddMonths(6),
                Token = Common.GenerateRandomString(35) + Guid.NewGuid()
            };

            if (await m_refreshTokenRepository.AddAsync(refreshToken) == null) {
                throw new Exception("Can't create refresh token!");
            }

            return new TokenDto {
                AccessToken = jwtToken,
                RefreshToken = refreshToken.Token
            };
        }

        private async Task<List<Claim>> GetAllValidClaims(User user) {
            var claims = new List<Claim> {
                new Claim("Id", user.Id),
                new Claim(JwtRegisteredClaimNames.Email.ToString(), user.Email),
                new Claim(JwtRegisteredClaimNames.Sub, user.Email),
                new Claim(JwtRegisteredClaimNames.Jti, Guid.NewGuid().ToString()),
            };

            // Getting the claims that have assigned to the user
            var userClaims = await m_userManager.GetClaimsAsync(user);
            claims.AddRange(userClaims);

            // Get the user role and add it to claims
            var userRoles = await m_userManager.GetRolesAsync(user);

            foreach (var userRole in userRoles) {
                var role = await m_roleManager.FindByNameAsync(userRole);
                if (role == null) {
                    continue;
                }

                claims.Add(new Claim(ClaimTypes.Role, userRole));
                var roleClaims = await m_roleManager.GetClaimsAsync(role);
                foreach (var roleClaim in roleClaims) {
                    claims.Add(roleClaim);
                }
            }

            return claims;
        }

        public async Task<bool> VerifyToken(TokenDto token) {
            var jwtTokenHandler = new JwtSecurityTokenHandler();
            try {
                var validationParameters = GetTokenValidationParameters();
                var tokenInVeryfication = jwtTokenHandler.ValidateToken(token.AccessToken, validationParameters, out var validatedToken);
                if (validatedToken is JwtSecurityToken jwtSecurityToken) {
                    var result = jwtSecurityToken.Header.Alg.Equals(SecurityAlgorithms.HmacSha256, StringComparison.InvariantCultureIgnoreCase);
                    if (result == false) {
                        return false;
                    }
                }

                var utcExpiryDate = long.Parse(tokenInVeryfication.Claims.FirstOrDefault(x => x.Type == JwtRegisteredClaimNames.Exp).Value);
                var expiryDate = UnixTimestampToDateTime(utcExpiryDate);
                var now = DateTime.UtcNow;
                if (expiryDate > now) {
                    throw new Exception("Token has not expired!");
                }

                var storedToken = await m_refreshTokenRepository.Get(x => x.Token == token.RefreshToken);
                if (storedToken == null) {
                    throw new Exception("Token doesn't exist");
                }

                if (storedToken.IsUsed) {
                    throw new Exception("Token was used");
                }

                if (storedToken.IsRevoked) {
                    throw new Exception("Token was revoked");
                }

                var jti = tokenInVeryfication.Claims.FirstOrDefault(x => x.Type == JwtRegisteredClaimNames.Jti).Value;
                if (storedToken.JwtId != jti) {
                    throw new Exception("Refresh token deoesn't match");
                }

                return true;
            } catch (Exception ex) {
                Console.WriteLine(ex.Message);

                return false;
            }
        }

        public TokenValidationParameters? GetTokenValidationParameters() {
            var key = GeyKeyBytes();
            if (key == null) {
                throw new Exception("Secret doesn't exist!");
            }

            return new TokenValidationParameters {
                ValidateIssuerSigningKey = true,
                ValidateIssuer = false,
                ValidateAudience = false,
                IssuerSigningKey = new SymmetricSecurityKey(key),
                ValidateLifetime = false
            };
        }

        private DateTime UnixTimestampToDateTime(long unixTimeStamp) {
            var dateTimeVal = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return dateTimeVal.AddSeconds(unixTimeStamp).ToUniversalTime();
        }

        private byte[] GeyKeyBytes() {
            return Encoding.ASCII.GetBytes(m_jwtConfig.Secret);
        }
    }
}
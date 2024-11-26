using Authorization.Domain.Models;
using Microsoft.AspNetCore.Authentication.JwtBearer;

namespace Authorization.Application.Services {
    public interface IJwtTokenService {
        String getSecret();
        string generateAccessToken(User user);
        IDictionary<string, object> verifyToken(string token);
        (string refreshToken, string jwt) generateRefreshToken(User user);
        //(string accToken, string refreshToken) generateRefreshToken(User user);
        JwtBearerOptions options(JwtBearerOptions jwtBearerOptions, string audience);
    }
}
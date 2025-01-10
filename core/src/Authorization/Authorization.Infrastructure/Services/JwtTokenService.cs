using System;
using System.Security.Cryptography;
using System.Text;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using JWT.Algorithms;
using JWT.Builder;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.IdentityModel.Tokens;

namespace Authorization.Infrastructure.Services {
    public class JwtTokenService : IJwtTokenService {
        private static readonly string SecretToken = "Superlongsupersecret!";

        public string getSecret() {
            return SecretToken;
        }

        public string generateAccessToken(User user) {
            return new JwtBuilder()
                .WithAlgorithm(new HMACSHA256Algorithm())
                .WithSecret(Encoding.ASCII.GetBytes(SecretToken))
                .AddClaim("exp", DateTimeOffset.UtcNow.AddMinutes(10).ToUnixTimeSeconds())
                .AddClaim("username", user.Username)
                .Encode();
        }

        public IDictionary<string, object> verifyToken(string token) {
            return new JwtBuilder()
                 .WithSecret(SecretToken)
                 .MustVerifySignature()
                 .Decode<IDictionary<string, object>>(token);
        }

        public (string refreshToken, string jwt) generateRefreshToken(User user) {
            var randomNumber = new byte[32];
            using (var rng = RandomNumberGenerator.Create()) {
                rng.GetBytes(randomNumber);
                Convert.ToBase64String(randomNumber);
            }

            var randomString = System.Text.Encoding.ASCII.GetString(randomNumber);

            string jwt = new JwtBuilder()
                .WithAlgorithm(new HMACSHA256Algorithm())
                .WithSecret(SecretToken)
                .AddClaim("exp", DateTimeOffset.UtcNow.AddHours(4).ToUnixTimeSeconds())
                .AddClaim("refresh", randomString)
                .AddClaim("username", user.Username)
                .Issuer("JwtExample")
                .Audience("refresh")
                .Encode();

            return (randomString, jwt);
        }

        public JwtBearerOptions options(JwtBearerOptions jwtBearerOptions, string audience) {
            jwtBearerOptions.RequireHttpsMetadata = false;
            jwtBearerOptions.SaveToken = true;
            jwtBearerOptions.TokenValidationParameters = new TokenValidationParameters {
                ValidateIssuerSigningKey = true,
                IssuerSigningKey = new SymmetricSecurityKey(Encoding.ASCII.GetBytes(SecretToken)),
                ValidateAudience = true,
                ValidAudience = audience,
                ValidateLifetime = true, //validate the expiration and not before values in the token
                ClockSkew = TimeSpan.FromMinutes(1) //1 minute tolerance for the expiration date
            };

            if (audience == "access") {
                jwtBearerOptions.Events = new JwtBearerEvents {
                    OnAuthenticationFailed = context => {
                        if (context.Exception.GetType() == typeof(SecurityTokenExpiredException)) {
                            if (!context.Response.Headers.ContainsKey("Token-Expired")) {
                                throw new Exception("Token expired key exist!");
                            }

                            context.Response.Headers.Add("Token-Expired", "true");
                        }

                        return Task.CompletedTask;
                    }
                };
            }


            return jwtBearerOptions;
        }
    }
}
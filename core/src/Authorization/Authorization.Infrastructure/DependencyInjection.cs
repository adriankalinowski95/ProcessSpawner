using System;
using System.IdentityModel.Tokens.Jwt;
using System.Text;
using Authorization.Application.Repositories;
using Authorization.Application.Services;
// using Authorization.Domain.Models;
using Authorization.Infrastructure.Configuration;
using Authorization.Infrastructure.Repositories;
using Authorization.Infrastructure.Services;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore.Metadata.Internal;
using Microsoft.Extensions.Configuration;
using Microsoft.IdentityModel.Tokens;
using Shared.Types.Db;

namespace Microsoft.Extensions.DependencyInjection;

public static class DependencyInjection {
    public static IServiceCollection AddInfrastructureForAuthorization(this IServiceCollection services, IConfiguration configuration) {
        AddServices(services, configuration);
        AuthenticationSettings(services, configuration);

        return services;
    }

    private static void AddServices(IServiceCollection services, IConfiguration configuration) {
        services.Configure<JwtConfig>(configuration.GetSection("JwtConfig"));

        services.AddScoped<IUserService, UserService>();
        services.AddScoped<IUserRepository, UserRepository>();

        services.AddScoped<IJwtService, JwtService>();
        services.AddScoped<IHashingService, HashingService>();
        services.AddScoped<IAuthManagementService, AuthManagementService>();
        services.AddScoped<IRefreshTokenRepository, RefreshTokenRepository>();

        services.AddAutoMapper(typeof(AutoMapperProfile));

        services.AddDefaultIdentity<Authorization.Domain.Models.User>()
                .AddRoles<IdentityRole>()
                .AddEntityFrameworkStores<DatabaseContext>();

        services.AddAuthorization(options => {
            options.AddPolicy(
                "WholeAccessPolicy",
                policy => policy.RequireClaim("access", "all")
           );
        });
    }

    private static void AuthenticationSettings(IServiceCollection services, IConfiguration configuration) {
        var key = Encoding.ASCII.GetBytes(configuration["JwtConfig:Secret"]);
        if (key == null) {
            throw new Exception("Secret doesn't exist!");
        }

        var tokenValidationParamters = new TokenValidationParameters {
            ValidateIssuerSigningKey = true,
            IssuerSigningKey = new SymmetricSecurityKey(key),
            ValidateIssuer = false,
            ValidateAudience = false,
            ValidateLifetime = true,
            RequireExpirationTime = true,
            ClockSkew = TimeSpan.Zero
        };

        services.AddAuthentication(options => {
            options.DefaultAuthenticateScheme = JwtBearerDefaults.AuthenticationScheme;
            options.DefaultScheme = JwtBearerDefaults.AuthenticationScheme;
            options.DefaultChallengeScheme = JwtBearerDefaults.AuthenticationScheme;
        })
        .AddJwtBearer(jwt => {
            jwt.SaveToken = true;
            jwt.TokenValidationParameters = tokenValidationParamters;
        });
    }
}
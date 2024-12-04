﻿using System.Text;
using core.Settings;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using Shared.Tools;

namespace Microsoft.Extensions.DependencyInjection;

public static class DependencyInjection {
    public static IServiceCollection AddSettings(this IServiceCollection services, IConfiguration configuration) {
        services.AddControllers();
        services.AddEndpointsApiExplorer();
        services.AddSwaggerGen();
        services.AddGrpc();

        var specificOrigins = configuration["AllowSpecificOrigins"];
        var clientAddress = configuration["ClientAddress"];
        services.AddCors(options => {
            options.AddPolicy(name: specificOrigins, builder => {
                builder.WithOrigins(clientAddress)
                .AllowAnyMethod()
                .AllowAnyHeader()
                .AllowAnyOrigin();
            });
        });

        services.AddHttpContextAccessor();

        var jwtIssuer = configuration.GetSection("Jwt:Issuer").Get<string>();
        var jwtKey = configuration.GetSection("Jwt:Key").Get<string>();

        services.Configure<MvcOptions>(options => {
            options.InputFormatters.Insert(0, new TextPlainInputFormatter());
        });

        services.AddControllersWithViews().AddNewtonsoftJson(options =>
            options.SerializerSettings.ReferenceLoopHandling = Newtonsoft.Json.ReferenceLoopHandling.Ignore
        );

        services.AddInfrastructureForAuthorization(configuration);

        services.AddDbContext<DatabaseContext>();
        services.AddTransient<DbContext, DatabaseContext>();

        services.AddSingleton<IServiceCollection>(provider => {
            return services;
        });

        services.AddLoggingDependencies(configuration);

        services.AddLogging(loggingBuilder => {
            loggingBuilder.ClearProviders();
            loggingBuilder.AddDataBaseLoggerProvider(configuration);
        });

        services.AddInfrastructureForProcessSpawner(configuration);

        return services;
    }
}
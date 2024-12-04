using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Infrastructure.Services;

namespace Microsoft.Extensions.DependencyInjection;

public static class DependencyInjection {
    public static IServiceCollection AddInfrastructureForProcessSpawner(this IServiceCollection services, IConfiguration configuration) {
        services.AddScoped<IManagerService, ManagerService>();
        services.AddGrpc();

        return services;
    }
}

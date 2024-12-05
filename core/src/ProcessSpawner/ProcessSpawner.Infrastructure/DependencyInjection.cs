using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Infrastructure.Repositories;
using ProcessSpawner.Infrastructure.Services;
using ProcessSpawner.Infrastructure.Tools;

namespace Microsoft.Extensions.DependencyInjection;

public static class DependencyInjection {
    public static IServiceCollection AddInfrastructureForProcessSpawner(this IServiceCollection services, IConfiguration configuration) {
        services.AddScoped<IManagerService, ManagerService>();
        services.AddScoped<IProcessInstanceRepository, ProcessInstanceRepository>();
        services.AddScoped<IProcessSpawningService, ProcessSpawningService>();
        services.AddGrpc();

        services.AddAutoMapper(typeof(AutoMapperProfile));

        return services;
    }
}

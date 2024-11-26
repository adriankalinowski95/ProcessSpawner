using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Shared.Tools.Logging.Repositories;

namespace Microsoft.Extensions.DependencyInjection;

public static class DependencyInjection {
    public static IServiceCollection AddLoggingDependencies(this IServiceCollection services, IConfiguration configuration) {
        services.AddTransient<ILoggerRepository, LoggerRepository>();

        return services;
    }
}
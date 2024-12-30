using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using ProcessSpawner.Application.Commands;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Infrastructure.Commands;
using ProcessSpawner.Infrastructure.Repositories;
using ProcessSpawner.Infrastructure.Services;
using ProcessSpawner.Infrastructure.Tools;
using Quartz;

namespace Microsoft.Extensions.DependencyInjection;

public static class DependencyInjection {
    public static IServiceCollection AddInfrastructureForProcessSpawner(this IServiceCollection services, IConfiguration configuration) {
        services.AddScoped<IProcessManagerRepository, ProcessManagerRepository>();
        services.AddScoped<IProcessManagerService, ProcessManagerService>();
        services.AddScoped<IProcessManagerConfigProvider, ProcessManagerConfigProvider>();
        services.AddScoped<IProcessManagerSpawnProcessCommand, ProcessManagerSpawnProcessCommand>();
        services.AddScoped<IProcessManagerQueryCommand, ProcessManagerQueryCommand>();
        services.AddScoped<IProcessManagerFinishProcessCommand, ProcessManagerFinishProcessCommand>();
        services.AddScoped<IProcessManagerInputService, ProcessMangerInputService>();
        services.AddScoped<ICoreCommandCommunicationService, CoreCommandCommunicationService>();

        services.AddScoped<IProcessEventRepository, ProcessEventRepository>();
        services.AddScoped<IProcessManagerCommunicationService, ProcessManagerCommunicationService>();

        services.AddScoped<IProcessInstanceRepository, ProcessInstanceRepository>();
        services.AddScoped<IProcessSpawningService, ProcessSpawningService>();
        services.AddGrpc();

        services.AddAutoMapper(typeof(AutoMapperProfile));

        services.AddQuartz(configure => {
            configure.UseMicrosoftDependencyInjectionJobFactory();

            var jobKey = new JobKey(nameof(ProcessStatusScheduler));

            configure
                .AddJob<ProcessStatusScheduler>(jobKey)
                .AddTrigger(
                    trigger => trigger.ForJob(jobKey).WithSimpleSchedule(
                        schedule => schedule.WithIntervalInSeconds(60).RepeatForever()));
        });

        services.AddQuartzHostedService(options => {
            options.WaitForJobsToComplete = true;
        });

        return services;
    }
}

﻿using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Infrastructure.Repositories;
using ProcessSpawner.Infrastructure.Services;
using ProcessSpawner.Infrastructure.Tools;
using Quartz;

namespace Microsoft.Extensions.DependencyInjection;

public static class DependencyInjection {
    public static IServiceCollection AddInfrastructureForProcessSpawner(this IServiceCollection services, IConfiguration configuration) {
        services.AddScoped<IManagerService, ManagerService>();
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
                        schedule => schedule.WithIntervalInSeconds(10).RepeatForever()));
        });

        services.AddQuartzHostedService(options => {
            options.WaitForJobsToComplete = true;
        });

        return services;
    }
}

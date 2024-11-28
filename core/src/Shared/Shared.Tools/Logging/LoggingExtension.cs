using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Shared.Tools.Logging;
using Shared.Tools.Logging.Repositories;
using Shared.Tools.Logging.Services;
using static Microsoft.AspNetCore.Hosting.Internal.HostingApplication;

public static class LoggingExtension {
    public static ILoggingBuilder AddDataBaseLogger(this ILoggingBuilder builder, IConfiguration configuration) {
        //var dbContext = builder.Services.BuildServiceProvider().GetService<DbContext>();
        //var loggerRepository = new LoggerRepository(dbContext);


        builder.Services.AddSingleton<ILoggerProvider, DataBaseLoggerProvider>();


        /*
        builder.Services.AddSingleton<ILoggerProvider>(provider => {
            var dbContext = builder.Services.BuildServiceProvider().GetService<DbContext>();
            var loggerRepository = new LoggerRepository(dbContext);

            return new DataBaseLoggerProvider(configuration, loggerRepository);
        });
        */


        return builder;
    }
}
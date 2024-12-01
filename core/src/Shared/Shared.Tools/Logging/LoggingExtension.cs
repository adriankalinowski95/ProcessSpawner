﻿using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Shared.Tools.Logging;
using static Microsoft.AspNetCore.Hosting.Internal.HostingApplication;

public static class LoggingExtension {
    public static ILoggingBuilder AddDataBaseLoggerProvider(this ILoggingBuilder builder, IConfiguration configuration) {
        builder.Services.AddSingleton<ILoggerProvider, DataBaseLoggerProvider>();

        return builder;
    }
}
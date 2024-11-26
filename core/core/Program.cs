//using core.src.Infrastructure.Services;

using System.Net;
using core.Settings;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Infrastructure.Api;
using Shared.Tools.Middlewares;

var builder = WebApplication.CreateBuilder(args);
var configuration = builder.Configuration;

builder.Services.AddSettings(builder.Configuration);

var staticIp = builder.Configuration["StaticKestrelIp"];
var staticPort = builder.Configuration["StaticKestrelPort"];
var isStaticAddressVerbExist = Boolean.TryParse(builder.Configuration["IsKestrelStaticAddress"], out bool isStaticAddress);

Action<IWebHostBuilder> setStaticIp = (webHostBuilder) => {
    webHostBuilder.ConfigureKestrel(serverOptions => {
        if (isStaticAddressVerbExist && isStaticAddress) {
            if (staticIp != null && staticIp.Length != 0 && staticPort != null && staticPort.Length > 0) {
                serverOptions.Listen(IPAddress.Parse(staticIp), Int32.Parse(staticPort), o => {
                    // o.Protocols = Microsoft.AspNetCore.Server.Kestrel.Core.HttpProtocols.Http2;
                });
            }
        } else {
            serverOptions.ListenLocalhost(5002, o => {
                o.Protocols = Microsoft.AspNetCore.Server.Kestrel.Core.HttpProtocols.Http2;
            });
        }

        serverOptions.ListenLocalhost(5000);
        serverOptions.ListenLocalhost(5001, listenOptions => {
            listenOptions.UseHttps();
        });
    });
};

setStaticIp(builder.WebHost);

var app = builder.Build();

builder.WebHost.ConfigureAppConfiguration((hostingContext, config) => {
    var exePath = AppContext.BaseDirectory;
    config.SetBasePath(exePath)
          .AddJsonFile("appsettings.json", optional: false, reloadOnChange: true)
          .AddJsonFile($"appsettings.{hostingContext.HostingEnvironment.EnvironmentName}.json", optional: true, reloadOnChange: true)
          .AddEnvironmentVariables();
});



// @Todo remove this and just repair a dbcontext
new RuntimeSeed(app).Seed();

app.MapGrpcService<ProcessSpawner.Infrastructure.Api.gRPC.ProcessManagerInputController>();
app.MapGrpcService<ProcessSpawner.Infrastructure.Api.Example.CoreCommandCommunicationController>();
app.MapGrpcService<ProcessSpawner.Infrastructure.Api.Example.CoreQueryCommunicationController>();

// app.MapGrpcService<CoreGrpcService>();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment()) {
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseCoreExtension(configuration);

app.UseExceptionHandling();

// app.UseHttpsRedirection();

app.UseAuthorization();

app.MapControllers();

app.Run();
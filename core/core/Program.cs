//using core.src.Infrastructure.Services;

using System.Net;
using Microsoft.AspNetCore.Mvc;
using Shared.Tools.Middlewares;

var builder = WebApplication.CreateBuilder(args);
var configuration = builder.Configuration;

builder.Services.AddSettings(builder.Configuration);

var staticIp = builder.Configuration["KestrelIp"];
var staticPort = builder.Configuration["KestrelPort"];

builder.WebHost.ConfigureKestrel(serverOptions => {
    if (staticIp != null && staticIp.Length != 0 && staticPort != null && staticPort.Length > 0) {
        serverOptions.Listen(IPAddress.Parse(staticIp), Int32.Parse(staticPort));
    }

    serverOptions.ListenLocalhost(5000);
    serverOptions.ListenLocalhost(5001, listenOptions => {
        listenOptions.UseHttps();
    });
    serverOptions.Listen(IPAddress.Parse(staticIp), 5002, o => {
        o.Protocols = Microsoft.AspNetCore.Server.Kestrel.Core.HttpProtocols.Http2;
    });
});

var app = builder.Build();

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
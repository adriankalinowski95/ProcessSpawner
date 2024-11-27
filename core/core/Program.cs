//using core.src.Infrastructure.Services;

using Microsoft.AspNetCore.Mvc;
using Shared.Tools.Middlewares;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddSettings(builder.Configuration);

var app = builder.Build();

//app.MapGrpcService<CoreGrpcService>();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment()) {
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseExceptionHandling();

app.UseHttpsRedirection();

app.UseAuthorization();

app.MapControllers();

app.Run();
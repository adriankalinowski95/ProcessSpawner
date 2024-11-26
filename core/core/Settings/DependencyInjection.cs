using core.Settings;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace Microsoft.Extensions.DependencyInjection;

public static class DependencyInjection {
    public static IServiceCollection AddSettings(this IServiceCollection services, IConfiguration configuration) {
        services.AddControllers();
        services.AddEndpointsApiExplorer();
        services.AddSwaggerGen();
        services.AddGrpc();
        services.AddHttpContextAccessor();

        services.Configure<MvcOptions>(options => {
            options.InputFormatters.Insert(0, new TextPlainInputFormatter());
        });

        services.AddControllersWithViews().AddNewtonsoftJson(options =>
            options.SerializerSettings.ReferenceLoopHandling = Newtonsoft.Json.ReferenceLoopHandling.Ignore
        );

        services.AddInfrastructureForAuthorization(configuration);

        services.AddDbContext<DatabaseContext>();
        services.AddScoped<DbContext, DatabaseContext>();

        // @Todo
        // Middleware

        return services;
    }
}
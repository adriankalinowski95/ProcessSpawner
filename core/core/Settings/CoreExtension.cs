using System;

public static class ExceceptionHandlingExtension {
    public static IApplicationBuilder UseCoreExtension(this IApplicationBuilder app, IConfiguration configuration) {
        var specificOrigins = configuration["AllowSpecificOrigins"];
        app.UseCors(specificOrigins);

        return app;
    }
}
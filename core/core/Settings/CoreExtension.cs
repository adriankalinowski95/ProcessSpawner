using System;

public static class ExceceptionHandlingExtension {
    public static IApplicationBuilder UseCoreExtension(this IApplicationBuilder app, IConfiguration configuration) {
        var specificOrigins = configuration["AllowSpecificOrigins"];
        if (specificOrigins == null) {
            throw new NullReferenceException("SpecificOrigins doesn't exist in config");
        }

        app.UseCors(specificOrigins);

        return app;
    }
}
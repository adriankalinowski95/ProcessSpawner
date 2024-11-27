using System.Text.Json;
using Microsoft.AspNetCore.Http;
using Shared.Generic.RestApi;
using Microsoft.Extensions.Logging;
using Microsoft.AspNetCore.Builder;
using Shared.Tools.Middlewares;

namespace Shared.Tools.Middlewares {
    public class ExceptionHandlingMiddleware {
        private readonly RequestDelegate m_next;
        private readonly ILogger<ExceptionHandlingMiddleware> m_logger;

        public ExceptionHandlingMiddleware(RequestDelegate next, ILogger<ExceptionHandlingMiddleware> logger) {
            m_next = next;
            m_logger = logger;
        }

        public async Task InvokeAsync(HttpContext context) {
            try {
                await m_next(context);
            } catch (Exception ex) {
                await WriteErrorToResponse(context, ex);
            }
        }

        private async Task WriteErrorToResponse(HttpContext context, Exception ex) {
            m_logger.LogError(ex, ex.Message);

            context.Response.ContentType = "application/json";
            var result = JsonSerializer.Serialize(new BaseResponse {
                Status = BaseResponseStatus.Error,
                ErrorMessage = RestUtils.FormatException(ex)
            });

            await context.Response.WriteAsync(result);
        }
    }
}

public static class ExceceptionHandlingExtension {
    public static IApplicationBuilder UseExceptionHandling(this IApplicationBuilder app) {
        return app.UseMiddleware<ExceptionHandlingMiddleware>();
    }
}
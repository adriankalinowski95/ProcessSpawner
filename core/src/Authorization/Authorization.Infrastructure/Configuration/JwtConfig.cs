using System;
namespace Authorization.Infrastructure.Configuration {
    public class JwtConfig {
        public required string Secret { get; set; }
    }
}
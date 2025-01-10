using System;
namespace Authorization.Application.DTOs {
    public class AuthenticationDto {
        public required string Username { get; set; }
        public required string Password { get; set; }
    }
}
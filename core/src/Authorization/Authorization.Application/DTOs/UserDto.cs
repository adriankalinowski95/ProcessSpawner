using System;
using System.Text.Json.Serialization;

namespace Authorization.Application.DTOs {
    public class UserDto {
        [JsonPropertyName("id")]
        public string Id { get; set; }

        [JsonPropertyName("email")]
        public required string Email { get; set; }
    }
}
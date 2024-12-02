using System;
using System.Text.Json.Serialization;

namespace Authorization.Application.DTOs {
    public class UserDto {
        [JsonPropertyName("id")]
        public int Id { get; set; }

        [JsonPropertyName("email")]
        public string Email { get; set; }
    }
}
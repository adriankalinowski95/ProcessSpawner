﻿using System;
using System.Text.Json.Serialization;

namespace Authorization.Application.DTOs {
    public class TokenDto {
        [JsonPropertyName("accessToken")]
        public required string AccessToken { get; set; }

        [JsonPropertyName("refreshToken")]
        public required string RefreshToken { get; set; }
    }
}
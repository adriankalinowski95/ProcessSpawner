using System;
using System.Text.Json.Serialization;

namespace Shared.Generic.RestApi {
    public class BaseFieldValidationResponse {
        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        [JsonPropertyName("field")]
        public required string Field { get; set; }

        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        [JsonPropertyName("code")]
        public required string Code { get; set; }

        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        [JsonPropertyName("message")]
        public required string Message { get; set; }
    }
}


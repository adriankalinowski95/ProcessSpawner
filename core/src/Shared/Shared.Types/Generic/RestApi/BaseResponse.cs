using System.Text.Json.Serialization;

namespace Shared.Generic.RestApi {
    public enum BaseResponseStatus {
        Ok,
        Error
    }

    public class BaseResponse {
        [JsonPropertyName("status")]
        public BaseResponseStatus? Status { get; set; }

        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        [JsonPropertyName("errorMessage")]
        public string? ErrorMessage { get; set; }
    }
}

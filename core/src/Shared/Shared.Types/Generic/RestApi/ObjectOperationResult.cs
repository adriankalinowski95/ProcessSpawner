using System;
using System.Text.Json.Serialization;

namespace Shared.Generic.RestApi {
    public class ObjectOperationResult<T> : ObjectResponse<T> {
        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        [JsonPropertyName("validationResult")]
        public List<BaseFieldValidationResponse> ValidationResult { get; set; } = new List<BaseFieldValidationResponse>();
    }
}
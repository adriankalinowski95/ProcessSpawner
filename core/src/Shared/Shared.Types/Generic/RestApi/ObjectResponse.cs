using System.Text.Json.Serialization;

namespace Shared.Generic.RestApi {
    public class ObjectResponse<T> : BaseResponse {
        [JsonPropertyName("object")]
        public T Object { get; set; }
    }
}

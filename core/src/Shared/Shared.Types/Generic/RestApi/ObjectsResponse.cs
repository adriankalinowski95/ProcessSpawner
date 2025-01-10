using System;
using System.Text.Json.Serialization;

namespace Shared.Generic.RestApi {
    public class ObjectsResponse<T> : BaseResponse {
        [JsonPropertyName("objects")]
        public List<T>? Objects { get; set; }
    }
}


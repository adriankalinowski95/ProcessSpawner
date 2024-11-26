using System.Text.Json.Serialization;

namespace Shared.Generic.RestApi {
    public class BasePaginationResponse<T> : BaseResponse {
        [JsonPropertyName("data")]
        public List<T> Data { get; set; }
        [JsonPropertyName("total")]
        public int Total { get; set; }
        [JsonPropertyName("pageNumber")]
        public int PageNumber { get; set; }
        [JsonPropertyName("pageSize")]
        public int PageSize { get; set; }
    }
}

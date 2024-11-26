using System;
using System.Text.Json.Serialization;
using Shared.Generic.RestApi;

namespace Shared.Types.Generic.RestApi {
    public class BasePaginationConfigResponse : BaseResponse {
        [JsonPropertyName("pageNumber")]
        public int PageNumber { get; set; }

        [JsonPropertyName("pageSize")]
        public int PageSize { get; set; }

        [JsonPropertyName("length")]
        public int Length { get; set; }
    }
}
using System.Text.Json.Serialization;

namespace Shared.Generic.RestApi {
    public class BasePaginationRequest {
        private static int MaxPageNumber = 10;
        private static int MaxPageSize = 100;

        public BasePaginationRequest() {
            PageNumber = 1;
            PageSize = 10;
        }

        [JsonConstructor]
        public BasePaginationRequest(int pageNumber, int pageSize) {
            PageNumber = GetCorrectedPageNumber(pageNumber);
            PageSize = GetCorrectedPageSize(pageSize);
        }

        [JsonPropertyName("pageNumber")]
        public int PageNumber { get; set; }

        [JsonPropertyName("pageSize")]
        public int PageSize { get; set; }

        private int GetCorrectedPageNumber(int pageNumber) {
            return pageNumber > MaxPageNumber ? MaxPageSize : pageNumber <= 0 ? 1 : pageNumber;
        }

        private int GetCorrectedPageSize(int pageSize) {
            return pageSize > MaxPageSize ? MaxPageSize : pageSize <= 0 ? 1 : pageSize;
        }
    }
}

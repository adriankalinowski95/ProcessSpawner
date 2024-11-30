using Shared.Generic.RestApi;
using Shared.Generic.Services.String;

namespace Shared.Generic.Services.String {
    public interface ICrudServiceWithPagination<T> : ICrudService<T> where T : class {
        Task<BasePaginationResponse<T>> GetWithPagination(int pageNumber, int pageSize);
    }
}
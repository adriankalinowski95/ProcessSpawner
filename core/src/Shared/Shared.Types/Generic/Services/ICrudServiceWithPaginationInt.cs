using Shared.Generic.RestApi;
using Shared.Generic.Services.Int;

namespace Shared.Generic.Services.Int {
    public interface ICrudServiceWithPagination<T> : ICrudService<T> where T : class {
        Task<BasePaginationResponse<T>> GetWithPagination(int pageNumber, int pageSize);
    }
}
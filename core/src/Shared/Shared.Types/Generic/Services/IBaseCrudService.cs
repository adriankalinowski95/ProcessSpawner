using Shared.Generic.RestApi;

namespace Shared.Generic.Services {
    public interface IBaseCrudService<T, IdType> {
        Task<ObjectOperationResult<T>> Get(IdType id);
        Task<ObjectsResponse<T>> GetAll();
        Task<BasePaginationResponse<T>> GetAll(int pageNumber, int pageSize);
        Task<ObjectOperationResult<T>> Create(T obj);
        Task<ObjectOperationResult<T>> Put(IdType id, T obj);
        Task<ObjectOperationResult<T>> Delete(IdType id);
    }

    public interface IBaseCrudService<Input, Output, IdType> {
        Task<ObjectOperationResult<Output>> Get(IdType id);
        Task<ObjectsResponse<Output>> GetAll();
        Task<BasePaginationResponse<Output>> GetAll(int pageNumber, int pageSize);
        Task<ObjectOperationResult<Output>> Create(Input obj);
        Task<ObjectOperationResult<Output>> Put(IdType id, Input obj);
        Task<ObjectOperationResult<Output>> Delete(IdType id);
    }
}
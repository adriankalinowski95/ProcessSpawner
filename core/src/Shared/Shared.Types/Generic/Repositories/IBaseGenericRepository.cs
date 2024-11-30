namespace Shared.Generic.Repositories {
    public interface IBaseGenericRepository<T, K> {
        Task<IEnumerable<T>> GetAllAsync();
        Task<T> GetByIdAsync(K id);
        Task<T> AddAsync(T entity);
        void Update(T entity);
        void Remove(T entity);
    }
}
using System.IO;
using System.Linq.Expressions;

namespace Shared.Generic.Repositories {
    public interface IBaseGenericRepository<T, K> {
        Task<T> Get(Expression<Func<T, bool>> predicate);
        Task<T> GetByIdAsync(K id);
        Task<IEnumerable<T>> GetAllAsync(Expression<Func<T, bool>>? predicate = null);
        Task<IEnumerable<T>> GetAllAsync(int pageNumber, int pageSize, Expression<Func<T, bool>>? predicate = null);
        Task<T> AddAsync(T entity);
        void Update(T entity);
        void Remove(T entity);
        Task<int> CountAsync(Expression<Func<T, bool>>? predicate = null);
    }
}
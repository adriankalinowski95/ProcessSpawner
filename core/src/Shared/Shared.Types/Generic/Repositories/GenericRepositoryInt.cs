using System;
using System.Linq.Expressions;
using Microsoft.EntityFrameworkCore;

namespace Shared.Generic.Repositories.Int {
    public class GenericRepository<T> : IGenericRepository<T> where T : class {
        protected readonly DbContext m_context;
        protected readonly DbSet<T> m_dbSet;

        public GenericRepository(DbContext context) {
            m_context = context;
            m_dbSet = m_context.Set<T>();
        }

        public virtual async Task<T> Get(Expression<Func<T, bool>> predicate) {
            return await m_dbSet.Where(predicate).FirstAsync();
        }

        public virtual async Task<T> GetByIdAsync(int id) {
            return await m_dbSet.FindAsync(id);
        }

        public virtual async Task<IEnumerable<T>> GetAllAsync() {
            return await m_dbSet.ToListAsync();
        }

        public virtual async Task<IEnumerable<T>> GetAllAsync(Expression<Func<T, bool>>? predicate = null) {
            if (predicate == null) {
                return await m_dbSet.ToListAsync();
            }

            return await m_dbSet.Where(predicate).ToListAsync();
        }

        public virtual async Task<IEnumerable<T>> GetAllAsync(int pageNumber, int pageSize, Expression<Func<T, bool>>? predicate = null) {
            if (predicate == null) {
                return await m_dbSet.Skip((pageNumber - 1) * pageSize)
                                    .Take(pageSize)
                                    .ToListAsync();
            }

            return await m_dbSet.Where(predicate)
                                .Skip((pageNumber - 1) * pageSize)
                                .Take(pageSize)
                                .ToListAsync();
        }

        public virtual async Task<T> AddAsync(T entity) {
            var obj = await m_dbSet.AddAsync(entity);
            await m_context.SaveChangesAsync();

            return obj.Entity;
        }

        public virtual async void Update(T entity) {
            var result = m_context.Update(entity);
            var res = m_context.SaveChanges();
        }

        public virtual void Remove(T entity) {
            m_dbSet.Remove(entity);
            m_context.SaveChangesAsync();
        }

        public Task<int> CountAsync(Expression<Func<T, bool>>? predicate = null) {
            if (predicate == null) {
                return m_dbSet.CountAsync();
            }

            return m_dbSet.Where(predicate).CountAsync();
        }
    }
}

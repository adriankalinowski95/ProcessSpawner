using System;
using Microsoft.EntityFrameworkCore;

namespace Shared.Generic.Repositories.Int {
    public class GenericRepository<T> : IGenericRepository<T> where T : class {
        protected readonly DbContext m_context;
        protected readonly DbSet<T> m_dbSet;

        public GenericRepository(DbContext context) {
            m_context = context;
            m_dbSet = m_context.Set<T>();
        }

        public virtual async Task<IEnumerable<T>> GetAllAsync() {
            return await m_dbSet.ToListAsync();
        }

        public virtual async Task<T> GetByIdAsync(int id) {
            return await m_dbSet.FindAsync(id);
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
    }
}

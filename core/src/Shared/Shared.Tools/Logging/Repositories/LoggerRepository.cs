using System.Linq.Expressions;
using Microsoft.EntityFrameworkCore;
using Shared.Tools.Logging.Models;
using Shared.Types.Db;
using static Microsoft.AspNetCore.Hosting.Internal.HostingApplication;

namespace Shared.Tools.Logging.Repositories {
    public class LoggerRepository : ILoggerRepository {
        protected DatabaseContext m_context;
        // protected readonly DbSet<Log> m_dbSet;

        public LoggerRepository(DatabaseContext context) {
            m_context = context;
            // m_context.ChangeTracker.
            // m_dbSet = m_context.Set<T>();
        }

        public virtual async Task<IEnumerable<Log>> GetAllAsync() {
            var dbSet = m_context.Set<Log>();
            return await dbSet.ToListAsync();
        }

        public virtual async Task<Log> GetByIdAsync(int id) {
            var dbSet = m_context.Set<Log>();
            return await dbSet.FindAsync(id);
        }

        public virtual async Task<Log> AddAsync(Log entity) {
            var dbSet = m_context.Set<Log>();

            var obj = await dbSet.AddAsync(entity);

            //m_context.SaveChanges();
            // @Todo Tutaj jest problem, bo zapisytwanie generuje logi. Z kolei logi, powoduja to, ze jest odpalany loger i tak sie robi nieskonczona petla
            // musi byc jakie rozroznienie dla loggerow czy cos.
            await m_context.SaveChangesAsync();

            return obj.Entity;
        }

        public virtual async void Update(Log entity) {
            var result = m_context.Update(entity);

            var res = m_context.SaveChanges();

            Console.Write(res);
        }

        public virtual void Remove(Log entity) {
            var dbSet = m_context.Set<Log>();

            dbSet.Remove(entity);

            m_context.SaveChangesAsync();
        }

        public Task<Log> Get(Expression<Func<Log, bool>> predicate) {
            throw new NotImplementedException();
        }

        public Task<IEnumerable<Log>> GetAllAsync(Expression<Func<Log, bool>>? predicate = null) {
            throw new NotImplementedException();
        }

        public Task<IEnumerable<Log>> GetAllAsync(int pageNumber, int pageSize, Expression<Func<Log, bool>>? predicate = null) {
            throw new NotImplementedException();
        }

        public Task<int> CountAsync(Expression<Func<Log, bool>>? predicate = null) {
            throw new NotImplementedException();
        }
    }
}
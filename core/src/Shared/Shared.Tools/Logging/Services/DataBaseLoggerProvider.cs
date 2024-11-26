using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Shared.Tools.Logging.Repositories;
using Shared.Types.Db;

namespace Shared.Tools.Logging {
    public class DataBaseLoggerProvider : ILoggerProvider {
        private readonly IConfiguration m_configuration;
        private readonly IServiceCollection m_serviceCollection;
        private readonly Mutex m_savingToDbMutex;
        private readonly DatabaseContext m_dbContext;
        private readonly LoggerRepository m_loggerRepository;

        public DataBaseLoggerProvider(IConfiguration configuration, IServiceCollection serviceCollection) {
            m_configuration = configuration;
            m_serviceCollection = serviceCollection;
            m_savingToDbMutex = new Mutex();

            // @Todo Rapir this shit. Right now, its craeting a databasecontext all the time, when logger is needed.
            m_dbContext = m_serviceCollection.BuildServiceProvider().GetService<DatabaseContext>();
            if (m_dbContext == null) {
                throw new NullReferenceException("DbContext is empty while loading logger!");
            }

            m_loggerRepository = new LoggerRepository(m_dbContext);
        }

        public ILogger CreateLogger(string categoryName) {
            return new DefaultDataBaseLogger(m_configuration, m_loggerRepository, (categoryName) => true, categoryName, m_savingToDbMutex);
        }

        public void Dispose() {
            // throw new NotImplementedException();
        }
    }
}
using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Shared.Tools.Logging.Repositories;
using Shared.Tools.Logging.Services;

namespace Shared.Tools.Logging {
    public class DataBaseLoggerProvider : ILoggerProvider {
        private readonly IConfiguration m_configuration;
        private readonly IServiceCollection m_serviceCollection;
        private DbContext m_dbContext;
        private Mutex mutex = new Mutex();
        // private readonly ILoggerRepository m_loggerRepository;

        /*
        public DataBaseLoggerProvider(IConfiguration configuration, ILoggerRepository loggerRepository) {
            m_configuration = configuration;
            m_loggerRepository = loggerRepository;
        }
        */

        public DataBaseLoggerProvider(IConfiguration configuration, IServiceCollection serviceCollection) {
            m_configuration = configuration;
            m_serviceCollection = serviceCollection;

            m_dbContext = m_serviceCollection.BuildServiceProvider().GetService<DbContext>();
            // var loggerRepository = new LoggerRepository(dbContext);
            // m_loggerRepository = loggerRepository;
        }

        public ILogger CreateLogger(string categoryName) {
            // builder.Services.BuildServiceProvider().GetService<DbContext>();
            //var dbContext = m_serviceCollection.BuildServiceProvider().GetService<DbContext>();
            var loggerRepository = new LoggerRepository(m_dbContext);
            //return new DefaultDataBaseLogger(m_configuration, m_loggerRepository, (categoryName) => true, categoryName);
            return new DefaultDataBaseLogger(m_configuration, loggerRepository, (categoryName) => true, categoryName, mutex);
        }

        public void Dispose() {
            throw new NotImplementedException();
        }
    }
}
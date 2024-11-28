using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Shared.Tools.Logging.Repositories;

namespace Shared.Tools.Logging.Services {
    public class DataBaseLoggerProviderWithoutDb : ILoggerProvider {
        private readonly IConfiguration m_configuration;
        private readonly ILoggingBuilder m_loggingBuilder;
        private readonly ILoggerRepository m_loggerRepository;

        public DataBaseLoggerProviderWithoutDb(IConfiguration configuration, ILoggingBuilder loggingBuilder) {
            m_configuration = configuration;
            m_loggingBuilder = loggingBuilder;
        }

        public ILogger CreateLogger(string categoryName) {
            var dbContext = m_loggingBuilder.Services.BuildServiceProvider().GetService<DbContext>();
            var loggerRepository = new LoggerRepository(dbContext);

            // m_loggerRepository = loggerRepository;

            return new DefaultDataBaseLoggerWithoutDb(m_configuration, (categoryName) => true, categoryName);
        }

        public void Dispose() {
            throw new NotImplementedException();
        }
    }
}


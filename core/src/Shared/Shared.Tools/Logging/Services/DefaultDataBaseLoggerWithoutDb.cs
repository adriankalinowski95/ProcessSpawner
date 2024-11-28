using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using Shared.Tools.Logging.Repositories;

namespace Shared.Tools.Logging.Services {
    public class DefaultDataBaseLoggerWithoutDb : ILogger {
        private readonly IConfiguration m_configuration;
        // private readonly ILoggerRepository m_loggerRepository;

        private readonly LogLevel m_logLevel;
        private readonly Func<string, bool> m_logLevelFunc;
        private readonly string m_categoryName;

        private SemaphoreSlim semaphore = new(1, 1);

        public DefaultDataBaseLoggerWithoutDb(IConfiguration configuration, Func<string, bool> logLevelFunc, string categoryName) {
            m_configuration = configuration;
            // m_loggerRepository = loggerRepository;

            var logLevelString = m_configuration["Logging:LogLevel:Default"];
            Enum.TryParse<LogLevel>(logLevelString, out m_logLevel);

            m_logLevelFunc = logLevelFunc;
            m_categoryName = categoryName;
        }

        public IDisposable? BeginScope<TState>(TState state) where TState : notnull {
            return default;
        }

        public bool IsEnabled(LogLevel logLevel) {
            if (logLevel < m_logLevel) {
                return false;
            }

            return m_logLevelFunc(m_categoryName);
        }

        public async void Log<TState>(LogLevel logLevel, EventId eventId, TState state, Exception? exception, Func<TState, Exception?, string> formatter) {
            var message = formatter(state, exception);

            var logModel = new Models.Log {
                Level = logLevel,
                CategoryName = m_categoryName,
                Message = message
            };

            await WriteLogToFile(logModel);
        }

        private async Task WriteLogToFile(Models.Log log) {
            try {
                await semaphore.WaitAsync();

                //m_loggerRepository.AddAsync(log);
            } finally {
                semaphore.Release();
            }
        }
    }
}


using System;
using System.Reflection.Emit;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using Shared.Generic.RestApi;
using Shared.Tools.Logging.Models;
using Shared.Tools.Logging.Repositories;

namespace Shared.Tools.Logging {
    public class DefaultDataBaseLogger : ILogger {
        private readonly IConfiguration m_configuration;
        private readonly ILoggerRepository m_loggerRepository;

        private readonly LogLevel m_logLevel;
        private readonly Func<string, bool> m_logLevelFunc;
        private readonly string m_categoryName;
        private Mutex m_mutex;
        // private SemaphoreSlim semaphore = new(1, 1);

        public DefaultDataBaseLogger(IConfiguration configuration, ILoggerRepository loggerRepository, Func<string, bool> logLevelFunc, string categoryName,
            Mutex mutex
            ) {
            m_configuration = configuration;
            m_loggerRepository = loggerRepository;

            var logLevelString = m_configuration["Logging:LogLevel:Default"];
            Enum.TryParse<LogLevel>(logLevelString, out m_logLevel);

            m_logLevelFunc = logLevelFunc;
            m_categoryName = categoryName;
            m_mutex = mutex;
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

            if (m_categoryName.Contains("EntityFrameworkCore") == true) {
                // Console.WriteLine(message);

                return;
            }

            var logModel = new Models.Log {
                Level = logLevel,
                CategoryName = m_categoryName,
                Message = message
            };

            await WriteLogToDataBase(logModel);
        }

        private async Task WriteLogToDataBase(Models.Log log) {
            var random = Random.Shared.NextInt64();
            try {
                Console.WriteLine("RandomBeforeMutex: " + random);

                m_mutex.WaitOne();

                await m_loggerRepository.AddAsync(log);
            } finally {
                m_mutex.ReleaseMutex();
                Console.WriteLine("RandomAfterMutex: " + random);
            }
        }
    }
}
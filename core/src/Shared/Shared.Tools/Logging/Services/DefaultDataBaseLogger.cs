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

        public DefaultDataBaseLogger(
            IConfiguration configuration,
            ILoggerRepository loggerRepository,
            Func<string, bool> logLevelFunc,
            string categoryName,
            Mutex mutex) {
            m_configuration = configuration;
            m_loggerRepository = loggerRepository;
            m_logLevelFunc = logLevelFunc;
            m_categoryName = categoryName;
            m_mutex = mutex;

            var logLevelString = m_configuration["Logging:LogLevel:Default"];
            Enum.TryParse<LogLevel>(logLevelString, out m_logLevel);
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

            // Prevent before cycle of saving to database / logging that
            if (IsCategoryFiltred(m_categoryName)) {
                return;
            }

            if (IsCategoryToPrint(m_categoryName)) {
                Console.WriteLine(message);
                return;
            }

            var logModel = new Models.Log {
                Level = logLevel,
                CategoryName = m_categoryName,
                Message = message
            };

            await WriteLogToDataBase(logModel);
        }

        private static bool IsCategoryFiltred(string categoryName) {
            return categoryName.Contains(Entiy_Framework_Core_Category_Name);
        }

        private static bool IsCategoryToPrint(string categoryName) {
            return categoryName.Contains(Microsoft_Hostring_Lifetime_Category_Name);
        }

        private async Task WriteLogToDataBase(Models.Log log) {
            try {
                m_mutex.WaitOne();

                await m_loggerRepository.AddAsync(log);
            } finally {
                m_mutex.ReleaseMutex();
            }
        }

        private const string Entiy_Framework_Core_Category_Name = "EntityFrameworkCore";
        private const string Microsoft_Hostring_Lifetime_Category_Name = "Microsoft.Hosting.Lifetime";
    }
}
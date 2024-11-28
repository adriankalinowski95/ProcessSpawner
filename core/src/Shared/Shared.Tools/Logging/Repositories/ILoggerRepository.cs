using System;
using Shared.Generic.Repositories.Int;
using Shared.Tools.Logging.Models;

namespace Shared.Tools.Logging.Repositories {
    public interface ILoggerRepository : IGenericRepository<Log> { }
}
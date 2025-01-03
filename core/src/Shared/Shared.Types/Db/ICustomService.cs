using System;
using Microsoft.EntityFrameworkCore;

namespace Shared.Types.Db {
    public interface ICustomService {
        void ConfigureModel(ModelBuilder modelBuilder);
    }
}
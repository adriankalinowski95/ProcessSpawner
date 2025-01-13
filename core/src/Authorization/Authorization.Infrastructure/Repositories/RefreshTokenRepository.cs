using System;
using Authorization.Domain.Models;
using Shared.Types.Db;

namespace Authorization.Infrastructure.Repositories {
    public class RefreshTokenRepository : Shared.Generic.Repositories.Int.GenericRepository<RefreshToken>, Application.Repositories.IRefreshTokenRepository {
        public RefreshTokenRepository(DatabaseContext dbContext) : base(dbContext) { }


    }
}
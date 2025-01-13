using System;
using Shared.Generic.Repositories.Int;

namespace Authorization.Application.Repositories {
    public interface IRefreshTokenRepository : IGenericRepository<Domain.Models.RefreshToken> { }
}
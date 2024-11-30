﻿using Authorization.Domain.Models;
using Shared.Generic.Repositories.Int;

namespace Authorization.Application.Services {
    public interface IUserRepository : IGenericRepository<Domain.Models.User> {
        public User GetByName(string name);
    }
}
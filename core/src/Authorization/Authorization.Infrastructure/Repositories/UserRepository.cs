using System;
using System.Security.Claims;
using Authorization.Domain.Models;
using Microsoft.EntityFrameworkCore;
using Shared.Types.Db;

namespace Authorization.Infrastructure.Repositories {
    public class UserRepository : Shared.Generic.Repositories.Int.GenericRepository<User>, Application.Services.IUserRepository {
        public UserRepository(DatabaseContext dbContext) : base(dbContext) {

        }

        public User GetByName(string name) {
            var user = m_dbSet.Where(user => user.Username == name).FirstOrDefault();
            if (user == null) {
                throw new KeyNotFoundException("Can't find user by name.");
            }

            return user;
        }
    }
}
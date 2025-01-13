using System;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using Microsoft.AspNetCore.Identity;
using Microsoft.Extensions.Logging;
using Shared.Generic.RestApi;

namespace Authorization.Infrastructure.Services {
    public class UserService : IUserService {
        private readonly ILogger<IUserService> m_logger;
        private readonly IUserRepository m_userRepository;
        private readonly UserManager<User> m_userManager;

        public UserService(ILogger<IUserService> logger, IUserRepository userRepository, UserManager<User> userManager) {
            m_logger = logger;
            m_userRepository = userRepository;
            m_userManager = userManager;
        }

        public async Task<ObjectOperationResult<User>> Get(string id) {
            var user = m_userManager.Users.Where(user => user.Id.CompareTo(id) == 0).First();
            if (user == null) {
                throw new KeyNotFoundException("Can't find user for id: " + id);
            }

            return new ObjectOperationResult<User> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = user
            };
        }

        public async Task<ObjectsResponse<User>> GetAll() {
            var users = await m_userRepository.GetAllAsync();

            return new ObjectsResponse<User> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Objects = users.ToList()
            };
        }

        public async Task<ObjectOperationResult<User>> Create(User obj) {
            var newUser = await m_userRepository.AddAsync(obj);
            if (newUser == null) {
                throw new Exception("Cant create algorithm!");
            }

            return new ObjectOperationResult<User> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = newUser
            };
        }

        public async Task<ObjectOperationResult<User>> Delete(string id) {
            var target = await m_userRepository.GetByIdAsync(id);
            if (target == null) {
                throw new Exception("Cant remove user!");
            }

            m_userRepository.Remove(target);

            return new ObjectOperationResult<User> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = target
            };
        }

        public Task<ObjectOperationResult<User>> Put(string id, User obj) {
            if (id != obj.Id) {
                throw new KeyNotFoundException("AlgorithmDto is different than id in param!");
            }

            m_userRepository.Update(obj);

            return Task.FromResult(new ObjectOperationResult<User> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = obj
            });
        }

        public Task<BasePaginationResponse<User>> GetAll(int pageNumber, int pageSize) {
            throw new NotImplementedException();
        }
    }
}
using System;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using Microsoft.Extensions.Logging;
using Shared.Generic.RestApi;

namespace Authorization.Infrastructure.Services {
    // @Todo I guess i should make a some wrapper for this
    // Like, all of my functions have a try/catch, so i should a make w rapper to minimalize code.
    // Or just a middleware??
    public class UserService : IUserService {
        private readonly ILogger<IUserService> m_logger;
        private IUserRepository m_userRepository;
        private IUserAuthenticationService m_userAuthenticationService;

        public UserService(IUserRepository userRepository, IUserAuthenticationService userAuthenticationService, ILogger<IUserService> logger) {
            m_userRepository = userRepository;
            m_userAuthenticationService = userAuthenticationService;
            m_logger = logger;
        }

        public async Task<ObjectOperationResult<User>> Get(int id) {
            var user = await m_userRepository.GetByIdAsync(id);
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

        public async Task<ObjectOperationResult<User>> Delete(int id) {
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

        public Task<ObjectOperationResult<User>> Put(int id, User obj) {
            if (id != obj.Id) {
                throw new Exception("AlgorithmDto is different than id in param!");
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
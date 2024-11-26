using System;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using Shared.Generic.RestApi;

namespace Authorization.Infrastructure.Services {
    // @Todo I guess i should make a some wrapper for this
    // Like, all of my functions have a try/catch, so i should a make w rapper to minimalize code.
    // Or just a middleware??
    public class UserService : IUserService {
        private IUserRepository m_userRepository;
        private IUserAuthenticationService m_userAuthenticationService;

        public UserService(IUserRepository userRepository, IUserAuthenticationService userAuthenticationService) {
            m_userRepository = userRepository;
            m_userAuthenticationService = userAuthenticationService;
        }

        public async Task<ObjectOperationResult<User>> Get(int id) {
            try {
                var user = await m_userRepository.GetByIdAsync(id);
                if (user == null) {
                    throw new Exception("Can't find user for id: " + id);
                }

                return new ObjectOperationResult<User> {
                    Status = BaseResponseStatus.Ok,
                    ErrorMessage = string.Empty,
                    Object = user
                };
            } catch (Exception ex) {
                return new ObjectOperationResult<User> { Status = BaseResponseStatus.Error, ErrorMessage = RestUtils.FormatException(ex) };
            }
        }

        public Task<ObjectsResponse<User>> GetAll() {
            throw new NotImplementedException();
        }

        public async Task<ObjectOperationResult<User>> Create(User obj) {
            try {
                var user = m_userAuthenticationService.GetCurrentUser();

                /*
                // @Todo to powinno byc zaimlmenetowane
                var algorithm = AlgorithmToDtoConverter.ConvertToModel(obj, user);
                var validator = new BaseValidatorService<Models.BotAlgorithm.BotAlgorithm>(m_botAlgorithmValidators.BotAlgorithmCreate);
                if (await validator.SingleObjectValidation<BotAlgorithmDto>(algorithm) is var failResult && failResult != null) {
                    return failResult;
                }
                */

                var newUser = await m_userRepository.AddAsync(obj);
                if (newUser == null) {
                    throw new Exception("Cant create algorithm!");
                }

                return new ObjectOperationResult<User> {
                    Status = BaseResponseStatus.Ok,
                    ErrorMessage = string.Empty,
                    Object = newUser
                };
            } catch (Exception ex) {
                return new ObjectOperationResult<User> { Status = BaseResponseStatus.Error, ErrorMessage = RestUtils.FormatException(ex) };
            }
        }

        public async Task<ObjectOperationResult<User>> Delete(int id) {
            try {
                var user = m_userAuthenticationService.GetCurrentUser();
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
            } catch (Exception ex) {
                return new ObjectOperationResult<User> { Status = BaseResponseStatus.Error, ErrorMessage = RestUtils.FormatException(ex) };
            }
        }

        public async Task<ObjectOperationResult<User>> Put(int id, User obj) {
            try {
                if (id != obj.Id) {
                    throw new Exception("AlgorithmDto is different than id in param!");
                }

                var user = m_userAuthenticationService.GetCurrentUser();

                /*
                @Todo 
                var algorithm = AlgorithmToDtoConverter.ConvertToModel(obj, user);
                var validator = new BaseValidatorService<Models.BotAlgorithm.BotAlgorithm>(m_botAlgorithmValidators.BotAlgorithmUpdate);
                if (await validator.SingleObjectValidation<BotAlgorithmDto>(algorithm) is var failResult && failResult != null) {
                    return failResult;
                }
                */

                m_userRepository.Update(obj);

                return new ObjectOperationResult<User> {
                    Status = BaseResponseStatus.Ok,
                    ErrorMessage = string.Empty,
                    Object = obj
                };
            } catch (Exception ex) {
                return new ObjectOperationResult<User> { Status = BaseResponseStatus.Error, ErrorMessage = RestUtils.FormatException(ex) };
            }
        }
    }
}
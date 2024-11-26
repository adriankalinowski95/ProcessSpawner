using System;
using Shared.Generic.Services.Int;

namespace Authorization.Application.Services {
    public interface IUserService : ICrudService<Domain.Models.User> { }
}
using System;
using Shared.Generic.Services.String;

namespace Authorization.Application.Services {
    public interface IUserService : ICrudService<Domain.Models.User> { }
}
using System.Security.Claims;
using Authorization.Application.DTOs;
using Authorization.Domain.Models;

namespace Authorization.Application.Services {
    public interface IUserAuthenticationService {
        public User GetCurrentUser();
        public TokenDto Login(AuthenticationDto authentication);
        public TokenDto RefreshToken(Claim userClaim, Claim refreshClaim);
    }
}
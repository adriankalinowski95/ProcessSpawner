using System;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System.Security.Claims;
using System.Xml.Linq;
using Shared.Generic.RestApi;
using Authorization.Application.DTOs;
using Microsoft.AspNetCore.Http.HttpResults;

namespace Authorization.Infrastructure.Api {
    [Route("api/[controller]")]
    [ApiController]
    public class TokensController : ControllerBase {
        private readonly ILogger<TokensController> m_logger;
        private readonly IUserRepository m_userService;
        private readonly IUserAuthenticationService m_userAuthenticationService;

        public TokensController(ILogger<TokensController> logger,
                                IUserRepository userService,
                                IUserAuthenticationService userAuthenticationService) {
            m_logger = logger;
            m_userService = userService;
            m_userAuthenticationService = userAuthenticationService;
        }

        [HttpPost("accesstoken", Name = "login")]
        public ObjectResponse<TokenDto> Login([FromBody] Authorization.Application.DTOs.AuthenticationDto auth) {
            var token = m_userAuthenticationService.Login(auth);

            return new ObjectResponse<TokenDto> {
                ErrorMessage = string.Empty,
                Status = BaseResponseStatus.Ok,
                Object = token
            };
        }

        [Authorize(AuthenticationSchemes = "refresh")]
        [HttpPut("accesstoken", Name = "refresh")]
        public ObjectResponse<TokenDto> Refresh() {
            var refreshtoken = User.Claims.FirstOrDefault(x => x.Type == "refresh");
            var username = User.Claims.FirstOrDefault(x => x.Type == "username");
            if (refreshtoken == null || username == null) {
                throw new UnauthorizedAccessException("Refresh token or username is empty!");
            }

            var token = m_userAuthenticationService.RefreshToken(username, refreshtoken);

            return new ObjectResponse<TokenDto> {
                ErrorMessage = string.Empty,
                Status = BaseResponseStatus.Ok,
                Object = token
            };
        }
    }
}
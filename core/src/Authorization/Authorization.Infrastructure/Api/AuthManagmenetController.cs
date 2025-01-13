using System;
using Authorization.Application.DTOs;
using Authorization.Application.Services;
using Authorization.Infrastructure.Configuration;
using Authorization.Infrastructure.Services;
using AutoMapper;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;
using Shared.Generic.RestApi;

namespace Authorization.Infrastructure.Api {
    [Route("api/[controller]")]
    [ApiController]
    public class AuthManagementController : ControllerBase {
        private readonly ILogger<AuthManagementController> m_logger;
        private readonly IAuthManagementService m_authManagementService;
        private readonly IMapper m_mapper;

        public AuthManagementController(ILogger<AuthManagementController> logger, IMapper mapper, IAuthManagementService authManagementService) {
            m_logger = logger;
            m_authManagementService = authManagementService;
            m_mapper = mapper;
        }

        [HttpPost("login")]
        public async Task<ObjectResponse<TokenDto>> Login([FromBody] Authorization.Application.DTOs.AuthenticationDto auth) {
            var token = await m_authManagementService.Login(auth);
            if (token == null) {
                throw new UnauthorizedAccessException();
            }

            return new ObjectResponse<TokenDto> {
                ErrorMessage = string.Empty,
                Status = BaseResponseStatus.Ok,
                Object = token
            };
        }

        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        [HttpPost("getUserByToken")]
        public async Task<ObjectResponse<UserDto>> GetByToken() {
            return new ObjectResponse<UserDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = await m_authManagementService.GetCurrentUserDto()
            };
        }

        [HttpPost("refreshToken")]
        public async Task<ObjectResponse<TokenDto>> RefreshToken([FromBody] TokenDto token) {
            return new ObjectResponse<TokenDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = await m_authManagementService.RefreshToken(token)
            };
        }
    }
}
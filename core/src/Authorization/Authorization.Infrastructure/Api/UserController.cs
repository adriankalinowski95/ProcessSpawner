using System;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System.Security.Claims;
using System.Xml.Linq;
using Shared.Generic.RestApi;
using Authorization.Application.DTOs;
using AutoMapper;

namespace Authorization.Infrastructure.Api {
    [Route("api/[controller]")]
    [ApiController]
    public class UserController : ControllerBase {
        // @Todo Repair of saving password to hash and change of controllers (they are returing user, while i needed to have a ObjectResponse)
        // 
        private readonly ILogger<UserController> m_logger;
        private readonly IUserService m_userService;
        private readonly IUserAuthenticationService m_userAuthenticationService;
        private readonly IMapper m_mapper;

        public UserController(ILogger<UserController> logger, IUserService userService, IUserAuthenticationService userAuthenticationService, IMapper mapper) {
            m_logger = logger;
            m_userService = userService;
            m_userAuthenticationService = userAuthenticationService;
            m_mapper = mapper;
        }

        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        [HttpGet]
        public async Task<ObjectsResponse<User>> GetAll() {
            return await m_userService.GetAll();
        }

        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        [HttpGet("{userId}")]
        public async Task<ObjectResponse<User>> Get([FromRoute] int userId) {
            return await m_userService.Get(userId);
        }

        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        [HttpPost]
        public async Task<ObjectResponse<User>> Create([FromBody] User user) {
            return await m_userService.Create(user);
        }

        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        [HttpPut("{userId}")]
        public async Task<ObjectOperationResult<User>> Put([FromRoute] int userId, [FromBody] User user) {
            return await m_userService.Put(userId, user);
        }

        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        [HttpDelete("{userId}")]
        public async Task<ObjectOperationResult<User>> Delete([FromRoute] int userId) {
            return await m_userService.Delete(userId);
        }

        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        [HttpPost("GetByToken", Name = "GetByToken")]
        public Task<ObjectResponse<UserDto>> GetByToken() {
            return Task.FromResult(new ObjectResponse<UserDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = m_mapper.Map<UserDto>(m_userAuthenticationService.GetCurrentUser())
            });
        }
    }
}
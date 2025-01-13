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
using Authorization.Domain.Enums;

namespace Authorization.Infrastructure.Api {
    [Route("api/[controller]")]
    [ApiController]
    [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme, Roles = nameof(Roles.Admin))]
    public class UserController : ControllerBase {
        private readonly ILogger<UserController> m_logger;
        private readonly IUserService m_userService;
        private readonly IMapper m_mapper;

        public UserController(ILogger<UserController> logger, IUserService userService, Mapper mapper) {
            m_logger = logger;
            m_userService = userService;
            m_mapper = mapper;
        }

        [HttpGet]
        public async Task<ObjectsResponse<User>> GetAll() {
            return await m_userService.GetAll();
        }

        [HttpGet("{userId}")]
        public async Task<ObjectResponse<User>> Get([FromRoute] string userId) {
            return await m_userService.Get(userId);
        }

        [HttpPost]
        public async Task<ObjectResponse<User>> Create([FromBody] User user) {
            return await m_userService.Create(user);
        }

        [HttpPut("{userId}")]
        public async Task<ObjectOperationResult<User>> Put([FromRoute] string userId, [FromBody] User user) {
            return await m_userService.Put(userId, user);
        }

        [HttpDelete("{userId}")]
        public async Task<ObjectOperationResult<User>> Delete([FromRoute] string userId) {
            return await m_userService.Delete(userId);
        }
    }
}
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

namespace Authorization.Infrastructure.Api {
    [Route("api/[controller]")]
    [ApiController]
    public class UserController : ControllerBase {
        private readonly ILogger<UserController> m_logger;
        private readonly IUserRepository m_userService;
        private readonly IUserAuthenticationService m_userAuthenticationService;

        public UserController(ILogger<UserController> logger, IUserRepository userService) {
            m_logger = logger;
            m_userService = userService;
        }

        [HttpGet]
        public async Task<IEnumerable<User>> GetAsync() {
            m_logger.Log(LogLevel.Information, "GET ALL!!!!");
            return await m_userService.GetAllAsync();
        }

        [HttpGet("{userId}", Name = "GetOne")]
        public async Task<User> GetOne([FromRoute] int userId) {
            return await m_userService.GetByIdAsync(userId);
        }

        [HttpPost]
        public async Task<IActionResult> CreateOneAsync([FromBody] User user) {
            user = await m_userService.AddAsync(user);

            return CreatedAtRoute("GetOne", new { userId = user.Id }, user);
        }

        [HttpPut("{userId}")]
        public User UpdateOne([FromRoute] string userId, [FromBody] User user) {
            m_userService.Update(user);

            return user;
        }

        [HttpDelete("{userId}")]
        public async Task<IActionResult> DeleteOneAsync([FromRoute] int userId) {
            var user = await m_userService.GetByIdAsync(userId);
            m_userService.Remove(user);

            return NoContent();
        }

        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        [HttpPost("getUserByToken", Name = "getUserByToken")]
        public User GetUserByToken() {
            try {
                return m_userAuthenticationService.GetCurrentUser();
            } catch (Exception ex) {
                m_logger.LogError(RestUtils.FormatException(ex));

                return null;
            }
        }
    }
}
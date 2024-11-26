using System;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System.Security.Claims;
using System.Xml.Linq;

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
        public IActionResult Login([FromBody] Authorization.Application.DTOs.AuthenticationDto auth) {
            try {
                return Ok(m_userAuthenticationService.Login(auth));
            } catch (Exception e) {
                return BadRequest(e);
            }
        }

        [Authorize(AuthenticationSchemes = "refresh")]
        [HttpPut("accesstoken", Name = "refresh")]
        public IActionResult Refresh() {
            Claim refreshtoken = User.Claims.FirstOrDefault(x => x.Type == "refresh");
            Claim username = User.Claims.FirstOrDefault(x => x.Type == "username");
            if (refreshtoken == null || username == null) {
                return BadRequest("Refresh token or username is empty!");
            }

            try {
                return Ok(m_userAuthenticationService.RefreshToken(username, refreshtoken));
            } catch (Exception e) {
                return BadRequest(e.Message);
            }
        }
    }
}
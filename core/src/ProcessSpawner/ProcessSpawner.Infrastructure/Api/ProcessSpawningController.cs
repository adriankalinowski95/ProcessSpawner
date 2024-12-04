using System;
using System.Net;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Services;
using Shared.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Api {
    [Route("api/[controller]")]
    [ApiController]
    public class ProcessSpawningController {
        private readonly ILogger<ProcessSpawningController> m_logger;
        private readonly IManagerService m_managerService;

        public ProcessSpawningController(ILogger<ProcessSpawningController> logger, IManagerService managerService) {
            m_logger = logger;
            m_managerService = managerService;
        }

        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectResponse<SpawnProcessResponseDto>> SpawnProcess([FromBody] SpawnProcessRequestDto spawnProcessRequestDto) {
            return await m_managerService.SpawnProcess(spawnProcessRequestDto);
        }
    }
}
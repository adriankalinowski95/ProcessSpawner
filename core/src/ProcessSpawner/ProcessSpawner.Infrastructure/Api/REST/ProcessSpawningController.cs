using System;
using System.Net;
using Authorization.Domain.Models;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Services.REST;
using Shared.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Api.REST {
    [Route("api/[controller]")]
    [ApiController]
    public class ProcessSpawningController : ControllerBase {
        private readonly ILogger<ProcessSpawningController> m_logger;
        private readonly IProcessSpawningService m_processSpawningService;

        public ProcessSpawningController(ILogger<ProcessSpawningController> logger, IProcessSpawningService processSpawningService) {
            m_logger = logger;
            m_processSpawningService = processSpawningService;
        }

        [HttpPost("spawn-process", Name = "SpawnProcess")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectResponse<ProcessInstanceDto>> SpawnProcess([FromBody] ProcessSpawnRequestDto spawnProcessRequestDto) {
            return await m_processSpawningService.SpawnProcess(spawnProcessRequestDto);
        }

        [HttpPost("finish-process/{id}", Name = "FinishProcess")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectResponse<ProcessInstanceDto>> FinishProcess(int id) {
            return await m_processSpawningService.FinishProcess(id);
        }
    }
}
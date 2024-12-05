using System;
using System.Net;
using Authorization.Domain.Models;
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
    public class ProcessSpawningController : ControllerBase {
        private readonly ILogger<ProcessSpawningController> m_logger;
        private readonly IProcessSpawningService m_processSpawningService;

        public ProcessSpawningController(ILogger<ProcessSpawningController> logger, IProcessSpawningService processSpawningService) {
            m_logger = logger;
            m_processSpawningService = processSpawningService;
        }

        [HttpPost]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectResponse<ProcessInstanceDto>> Create([FromBody] ProcessSpawnRequestDto spawnProcessRequestDto) {
            return await m_processSpawningService.Create(spawnProcessRequestDto);
        }

        [HttpGet("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessInstanceDto>> Get(int id) {
            return await m_processSpawningService.Get(id);
        }

        [HttpPut("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessInstanceDto>> Put(int id, [FromBody] ProcessInstanceDto algorithmDto) {
            return await m_processSpawningService.Put(id, algorithmDto);
        }

        [HttpDelete("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessInstanceDto>> Delete(int id) {
            return await m_processSpawningService.Delete(id);
        }
    }
}
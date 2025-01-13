using System;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Services.REST;
using Shared.Generic.RestApi;
using System.Xml.Linq;
using Authorization.Domain.Enums;

namespace ProcessSpawner.Infrastructure.Api.REST {
    [Route("api/[controller]")]
    [ApiController]
    [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme, Roles = nameof(Roles.Admin))]
    //[Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme, Policy = "WholeAccessPolicy")]
    public class ProcessManagerController : ControllerBase {
        private readonly ILogger<ProcessManagerController> m_logger;
        private readonly IProcessManagerService m_processManagerService;

        public ProcessManagerController(ILogger<ProcessManagerController> logger, IProcessManagerService processManagerService) {
            m_logger = logger;
            m_processManagerService = processManagerService;
        }

        [HttpPost]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectResponse<ProcessManagerDto>> Create([FromBody] ProcessManagerDto spawnProcessRequestDto) {
            return await m_processManagerService.Create(spawnProcessRequestDto);
        }

        [HttpGet]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectsResponse<ProcessManagerDto>> Get() {
            return await m_processManagerService.GetAll();
        }

        [HttpGet("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessManagerDto>> Get(int id) {
            return await m_processManagerService.Get(id);
        }

        [HttpPut("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessManagerDto>> Put(int id, [FromBody] ProcessManagerDto processInstanceDto) {
            return await m_processManagerService.Put(id, processInstanceDto);
        }

        [HttpDelete("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessManagerDto>> Delete(int id) {
            return await m_processManagerService.Delete(id);
        }
    }
}


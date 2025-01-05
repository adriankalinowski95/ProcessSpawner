using System;
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
    public class ProcessInstanceController : ControllerBase {
        private readonly ILogger<ProcessInstanceController> m_logger;
        private readonly IProcessInstanceService m_processInstanceService;

        public ProcessInstanceController(ILogger<ProcessInstanceController> logger, IProcessInstanceService processInstanceService) {
            m_logger = logger;
            m_processInstanceService = processInstanceService;
        }

        [HttpGet]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectsResponse<ProcessInstanceDto>> Get() {
            return await m_processInstanceService.GetAll();
        }

        [HttpGet("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessInstanceDto>> Get(int id) {
            return await m_processInstanceService.Get(id);
        }

        [HttpPut("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessInstanceDto>> Put(int id, [FromBody] ProcessInstanceDto processInstanceDto) {
            return await m_processInstanceService.Put(id, processInstanceDto);
        }

        [HttpDelete("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessInstanceDto>> Delete(int id) {
            return await m_processInstanceService.Delete(id);
        }
    }
}


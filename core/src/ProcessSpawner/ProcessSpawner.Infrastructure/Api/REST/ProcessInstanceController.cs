using System;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Services.REST;
using ProcessSpawner.Infrastructure.Services.REST;
using Shared.Generic.RestApi;
using Shared.Types.Generic.RestApi;

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
        public async Task<BasePaginationResponse<ProcessInstanceDto>> Get([FromQuery] int pageNumber, [FromQuery] int pageSize) {
            return await m_processInstanceService.GetAll(pageNumber, pageSize);
        }

        [HttpGet("user/{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<BasePaginationResponse<ProcessInstanceDto>> GetByUserId(int id, [FromQuery] int pageNumber, [FromQuery] int pageSize) {
            return await m_processInstanceService.GetByUserId(id, pageNumber, pageSize);
        }

        [HttpGet("manager/{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<BasePaginationResponse<ProcessInstanceDto>> GetByManagerId(int id, [FromQuery] int pageNumber, [FromQuery] int pageSize) {
            return await m_processInstanceService.GetByManagerId(id, pageNumber, pageSize);
        }

        [HttpGet("user/pagination-config/{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<BasePaginationConfigResponse> GetPaginationConfigForUser(int id) {
            return await m_processInstanceService.GetPaginationConfigForUser(id);
        }

        [HttpGet("manager/pagination-config/{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<BasePaginationConfigResponse> GetPaginationConfigForManager(int id) {
            return await m_processInstanceService.GetPaginationConfigForManager(id);
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


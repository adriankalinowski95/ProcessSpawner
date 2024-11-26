using System;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Services.REST;
using Shared.Generic.RestApi;
using Shared.Types.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Api.REST {
    [Route("api/[controller]")]
    [ApiController]
    public class ProcessEventController : ControllerBase {
        private readonly ILogger<ProcessEventController> m_logger;
        private readonly IProcessEventService m_processEventService;

        public ProcessEventController(ILogger<ProcessEventController> logger, IProcessEventService processEventService) {
            m_logger = logger;
            m_processEventService = processEventService;
        }

        [HttpGet]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<BasePaginationResponse<ProcessEventDto>> Get([FromQuery] int pageNumber, [FromQuery] int pageSize) {
            return await m_processEventService.GetAll(pageNumber, pageSize);
        }

        [HttpGet("process/{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<BasePaginationResponse<ProcessEventDto>> GetByProcessId(int id, [FromQuery] int pageNumber, [FromQuery] int pageSize) {
            return await m_processEventService.GetByProcessInstanceId(id, pageNumber, pageSize);
        }

        [HttpGet("process/pagination-config/{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<BasePaginationConfigResponse> GetPaginationConfigForProcess(int id) {
            return await m_processEventService.GetPaginationConfig(id);
        }

        [HttpGet("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessEventDto>> Get(int id) {
            return await m_processEventService.Get(id);
        }

        [HttpPut("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessEventDto>> Put(int id, [FromBody] ProcessEventDto processEventDto) {
            return await m_processEventService.Put(id, processEventDto);
        }

        [HttpDelete("{id}")]
        [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme)]
        public async Task<ObjectOperationResult<ProcessEventDto>> Delete(int id) {
            return await m_processEventService.Delete(id);
        }
    }
}


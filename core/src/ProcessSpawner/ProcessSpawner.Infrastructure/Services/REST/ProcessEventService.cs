using System;
using Authorization.Application.Services;
using AutoMapper;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services.REST;
using ProcessSpawner.Infrastructure.Repositories;
using Shared.Generic.RestApi;
using Shared.Types.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Services.REST {
    public class ProcessEventService : IProcessEventService {
        public readonly ILogger<ProcessEventService> m_logger;
        public readonly IProcessEventRepository m_processEventRepository;
        public readonly IUserAuthenticationService m_userAuthenticationService;
        private readonly IMapper m_mapper;

        public ProcessEventService(ILogger<ProcessEventService> logger, IProcessEventRepository processEventRepository, IMapper mapper) {
            m_logger = logger;
            m_processEventRepository = processEventRepository;
            m_mapper = mapper;
        }

        public async Task<BasePaginationResponse<ProcessEventDto>> GetByProcessInstanceId(int processInstanceId, int pageNumber, int pageSize) {
            var processes = await m_processEventRepository.GetAllAsync(pageNumber, pageSize, processEvent => processEvent.ProcessInstanceId == processInstanceId);
            var recordsCount = await m_processEventRepository.CountAsync(processEvent => processEvent.ProcessInstanceId == processInstanceId);
            var processEvents = processes.Select(process => m_mapper.Map<ProcessEventDto>(process)).ToList();

            return new BasePaginationResponse<ProcessEventDto> {
                Data = processEvents,
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                PageNumber = pageNumber,
                PageSize = pageSize,
                Total = recordsCount
            };
        }

        public Task<ObjectOperationResult<ProcessEventDto>> Get(int id) {
            throw new NotImplementedException();
        }

        public Task<ObjectsResponse<ProcessEventDto>> GetAll() {
            throw new NotImplementedException();
        }

        public Task<BasePaginationResponse<ProcessEventDto>> GetAll(int pageNumber, int pageSize) {
            throw new NotImplementedException();
        }

        public Task<ObjectOperationResult<ProcessEventDto>> Create(ProcessEventDto obj) {
            throw new NotImplementedException();
        }

        public Task<ObjectOperationResult<ProcessEventDto>> Put(int id, ProcessEventDto obj) {
            throw new NotImplementedException();
        }

        public Task<ObjectOperationResult<ProcessEventDto>> Delete(int id) {
            throw new NotImplementedException();
        }

        public async Task<BasePaginationConfigResponse> GetPaginationConfig(int processInstanceId) {
            var recordsCount = await m_processEventRepository.CountAsync(process => process.ProcessInstanceId == processInstanceId);

            return new BasePaginationConfigResponse {
                PageSize = 10,
                Length = recordsCount,
                PageNumber = 1,
                Status = BaseResponseStatus.Ok
            };
        }
    }
}


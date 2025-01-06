using System;
using Authorization.Application.Services;
using Authorization.Domain.Models;
using AutoMapper;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Commands;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services.Common;
using ProcessSpawner.Application.Services.REST;
using Shared.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Services.REST {
    public class ProcessInstanceService : IProcessInstanceService {
        public readonly IConfiguration m_configuration;
        public readonly ILogger<ProcessInstanceService> m_logger;
        public readonly IUserAuthenticationService m_userAuthenticationService;
        public readonly IProcessInstanceRepository m_processInstanceRepository;
        public readonly IProcessManagerService m_processManagerService;
        private readonly IMapper m_mapper;

        public ProcessInstanceService(
            IConfiguration configuration,
            ILogger<ProcessInstanceService> logger,
            IUserAuthenticationService userAuthenticationService,
            IProcessInstanceRepository processInstanceRepository,
            IProcessManagerService processManagerService,
            IMapper mapper) {
            m_configuration = configuration;
            m_logger = logger;
            m_userAuthenticationService = userAuthenticationService;
            m_processInstanceRepository = processInstanceRepository;
            m_processManagerService = processManagerService;
            m_mapper = mapper;
        }

        public Task<ObjectOperationResult<ProcessInstanceDto>> Create(ProcessInstanceDto obj) {
            throw new NotImplementedException();
        }

        public async Task<ObjectOperationResult<ProcessInstanceDto>> Delete(int id) {
            var processInstance = await m_processInstanceRepository.GetByIdAsync(id);
            if (processInstance == null) {
                throw new KeyNotFoundException("Can't find process instace for id: " + id);
            }

            m_processInstanceRepository.Remove(processInstance);

            return new ObjectOperationResult<ProcessInstanceDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = m_mapper.Map<ProcessInstanceDto>(processInstance),
            };
        }

        public async Task<ObjectOperationResult<ProcessInstanceDto>> Get(int id) {
            var processInstance = await m_processInstanceRepository.GetByIdAsync(id);
            if (processInstance == null) {
                throw new KeyNotFoundException("Can't find process instace for id: " + id);
            }

            return new ObjectOperationResult<ProcessInstanceDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = m_mapper.Map<ProcessInstanceDto>(processInstance),
            };
        }

        public async Task<ObjectsResponse<ProcessInstanceDto>> GetAll() {
            // @Todo Return per user, not whole
            var user = m_userAuthenticationService.GetCurrentUser();

            var processesInstances = await m_processInstanceRepository.GetAllAsync();
            if (processesInstances == null) {
                throw new Exception("Can't get processes");
            }

            return new ObjectsResponse<ProcessInstanceDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Objects = processesInstances.Select(processInstance => m_mapper.Map<ProcessInstanceDto>(processInstance)).ToList()
            };
        }

        public async Task<BasePaginationResponse<ProcessInstanceDto>> GetAll(int pageNumber, int pageSize) {
            // @Todo change this to service
            var request = new BasePaginationRequest(pageNumber, pageSize);
            var processes = await m_processInstanceRepository.GetAllAsync(request.PageNumber, request.PageSize);
            var processesDto = processes.Select(process => m_mapper.Map<ProcessInstanceDto>(process)).ToList();

            return new BasePaginationResponse<ProcessInstanceDto> {
                Data = processesDto,
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                PageNumber = pageNumber,
                PageSize = pageSize
            };
        }

        public async Task<BasePaginationResponse<ProcessInstanceDto>> GetByUserId(int userId, int pageNumber, int pageSize) {
            var processes = await m_processInstanceRepository.GetAllAsync(pageNumber, pageSize, process => process.UserId == userId);
            var processesDto = processes.Select(process => m_mapper.Map<ProcessInstanceDto>(process)).ToList();

            return new BasePaginationResponse<ProcessInstanceDto> {
                Data = processesDto,
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                PageNumber = pageNumber,
                PageSize = pageSize
            };
        }

        public async Task<BasePaginationResponse<ProcessInstanceDto>> GetByManagerId(int managerId, int pageNumber, int pageSize) {
            var processes = await m_processInstanceRepository.GetAllAsync(pageNumber, pageSize, process => process.ProcessManagerId == managerId);
            var processesDto = processes.Select(process => m_mapper.Map<ProcessInstanceDto>(process)).ToList();

            return new BasePaginationResponse<ProcessInstanceDto> {
                Data = processesDto,
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                PageNumber = pageNumber,
                PageSize = pageSize
            };
        }

        public async Task<ObjectOperationResult<ProcessInstanceDto>> Put(int id, ProcessInstanceDto obj) {
            // @Todo Validator
            throw new NotImplementedException();
        }
    }
}


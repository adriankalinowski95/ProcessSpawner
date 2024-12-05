using System;
using Authorization.Application.DTOs;
using Authorization.Application.Services;
using AutoMapper;
using Grpc.Core;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services;
using ProcessSpawner.Domain.Models;
using Shared.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Services {
    public class ProcessSpawningService : IProcessSpawningService {
        public readonly IConfiguration m_configuration;
        public readonly ILogger<ProcessSpawningService> m_logger;
        public readonly IManagerService m_managerService;
        public readonly IUserAuthenticationService m_userAuthenticationService;
        public readonly IProcessInstanceRepository m_processInstanceRepository;
        private readonly IMapper m_mapper;

        public ProcessSpawningService(IConfiguration configuration,
            ILogger<ProcessSpawningService> logger,
            IManagerService managerService,
            IUserAuthenticationService userAuthenticationService,
            IProcessInstanceRepository processInstanceRepository,
            IMapper mapper) {
            m_configuration = configuration;
            m_logger = logger;
            m_managerService = managerService;
            m_userAuthenticationService = userAuthenticationService;
            m_processInstanceRepository = processInstanceRepository;
            m_mapper = mapper;
        }

        public async Task<ObjectOperationResult<ProcessInstanceDto>> Create(SpawnProcessRequestDto obj) {
            // @Todo Validator

            var spawnProcessResponse = await m_managerService.SpawnProcess(obj);
            if (!spawnProcessResponse.success) {
                return new ObjectOperationResult<ProcessInstanceDto> {
                    ErrorMessage = spawnProcessResponse.message,
                    Status = BaseResponseStatus.Error
                };
            }

            var procesInstance = new ProcessInstance {
                ProcessType = obj.process_type,
                ProcessId = spawnProcessResponse.process_id,
                Parameters = obj.parameters,
                Status = Domain.Enums.ProcessStatus.Started
            };

            procesInstance = await m_processInstanceRepository.AddAsync(procesInstance);

            return new ObjectOperationResult<ProcessInstanceDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = m_mapper.Map<ProcessInstanceDto>(procesInstance)
            };
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

        public async Task<ObjectOperationResult<ProcessInstanceDto>> Put(int id, ProcessInstanceDto obj) {
            // @Todo Validator

            throw new NotImplementedException();
        }
    }
}
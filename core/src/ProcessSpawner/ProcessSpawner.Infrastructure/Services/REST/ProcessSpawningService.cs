using System;
using Authorization.Application.DTOs;
using Authorization.Application.Services;
using AutoMapper;
using Grpc.Core;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.Commands;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services.Common;
using ProcessSpawner.Application.Services.REST;
using Shared.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Services {
    public class ProcessSpawningService : IProcessSpawningService {
        public readonly IConfiguration m_configuration;
        public readonly ILogger<ProcessSpawningService> m_logger;
        public readonly IProcessManagerConfigProvider m_processManagerConfigProvider;
        public readonly IProcessManagerSpawnProcessCommand m_processMangerSpawningCommunication;
        public readonly IProcessManagerFinishProcessCommand m_processManagerFinishProcessCommand;
        public readonly IUserAuthenticationService m_userAuthenticationService;
        public readonly IProcessInstanceRepository m_processInstanceRepository;
        public readonly IProcessManagerUtilsService m_processManagerUtilsService;
        private readonly IMapper m_mapper;

        public ProcessSpawningService(IConfiguration configuration,
            ILogger<ProcessSpawningService> logger,
            IProcessManagerConfigProvider processManagerConfigProvider,
            IProcessManagerSpawnProcessCommand processMangerSpawningCommunication,
            IProcessManagerFinishProcessCommand processManagerFinishProcessCommand,
            IUserAuthenticationService userAuthenticationService,
            IProcessInstanceRepository processInstanceRepository,
            IProcessManagerUtilsService processManagerUtilsService,
            IMapper mapper) {
            m_configuration = configuration;
            m_logger = logger;
            m_processManagerConfigProvider = processManagerConfigProvider;
            m_processMangerSpawningCommunication = processMangerSpawningCommunication;
            m_processManagerFinishProcessCommand = processManagerFinishProcessCommand;
            m_userAuthenticationService = userAuthenticationService;
            m_processInstanceRepository = processInstanceRepository;
            m_processManagerUtilsService = processManagerUtilsService;
            m_mapper = mapper;
        }

        // @Todo separate this
        public async Task<ObjectOperationResult<ProcessInstanceDto>> Create(ProcessSpawnRequestDto obj) {
            // @Todo Validator
            var spawnProcessResponse = await m_processMangerSpawningCommunication.SpawnProcess(m_processManagerConfigProvider.GetConfig(), obj);
            if (!spawnProcessResponse.success) {
                throw new Exception(spawnProcessResponse.message);
            }

            var processManager = await m_processManagerUtilsService.GetAvailableProcessManager();
            if (processManager == null) {
                throw new Exception("Process manager doesn't exist!");
            }

            var procesInstance = new Domain.Models.ProcessInstance {
                ProcessId = spawnProcessResponse.process_id,
                InternalId = spawnProcessResponse.internal_id,
                ProcessType = obj.process_type,
                Parameters = obj.parameters,
                Status = Domain.Enums.ProcessStatus.Started,
                CreatedTimeMs = DateTimeOffset.FromUnixTimeMilliseconds(spawnProcessResponse.created_time_ms).UtcDateTime,
                LastUpdateTimeMs = DateTimeOffset.FromUnixTimeMilliseconds(spawnProcessResponse.last_update_time_ms).UtcDateTime,
                ProcessManagerId = processManager.Id
            };

            procesInstance = await m_processInstanceRepository.AddAsync(procesInstance);

            return new ObjectOperationResult<ProcessInstanceDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = m_mapper.Map<ProcessInstanceDto>(procesInstance)
            };
        }

        public async Task<ObjectOperationResult<ProcessInstanceDto>> FinishProcess(int id) {
            var processInstance = await m_processInstanceRepository.GetByIdAsync(id);
            if (processInstance == null) {
                throw new Exception("Process instance doesn't exist!");
            }

            var result = await m_processManagerFinishProcessCommand.FinishProcess(m_processManagerConfigProvider.GetConfig(), new FinishProcessRequestDto(processInstance.InternalId));
            if (!result.success) {
                throw new Exception(result.message);
            }

            processInstance.Status = Domain.Enums.ProcessStatus.Finished;
            m_processInstanceRepository.Update(processInstance);

            return new ObjectOperationResult<ProcessInstanceDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = m_mapper.Map<ProcessInstanceDto>(processInstance)
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
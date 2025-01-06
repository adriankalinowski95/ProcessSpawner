using System;
using Authorization.Application.Services;
using AutoMapper;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services.Common;
using ProcessSpawner.Application.Services.REST;
using ProcessSpawner.Infrastructure.Repositories;
using Shared.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Services.REST {
    public class ProcessManagerService : IProcessManagerService {
        public readonly ILogger<ProcessManagerService> m_logger;
        public readonly IProcessManagerRepository m_processManagerRepository;
        public readonly IUserAuthenticationService m_userAuthenticationService;
        private readonly IMapper m_mapper;

        public ProcessManagerService(ILogger<ProcessManagerService> logger, IProcessManagerRepository processManagerRepository, IMapper mapper) {
            m_logger = logger;
            m_processManagerRepository = processManagerRepository;
            m_mapper = mapper;
        }

        public Task<ObjectOperationResult<ProcessManagerDto>> Create(ProcessManagerDto obj) {
            throw new NotImplementedException();
        }

        public Task<ObjectOperationResult<ProcessManagerDto>> Delete(int id) {
            throw new NotImplementedException();
        }

        public Task<ObjectOperationResult<ProcessManagerDto>> Get(int id) {
            throw new NotImplementedException();
        }

        public async Task<ObjectsResponse<ProcessManagerDto>> GetAll() {
            var processManagers = await m_processManagerRepository.GetAllAsync();
            if (processManagers == null) {
                throw new Exception("Can't get process managers");
            }

            return new ObjectsResponse<ProcessManagerDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Objects = processManagers.Select(processManager => m_mapper.Map<ProcessManagerDto>(processManager)).ToList()
            };
        }

        public Task<BasePaginationResponse<ProcessManagerDto>> GetAll(int pageNumber, int pageSize) {
            throw new NotImplementedException();
        }

        public Task<ObjectOperationResult<ProcessManagerDto>> Put(int id, ProcessManagerDto obj) {
            throw new NotImplementedException();
        }
    }
}
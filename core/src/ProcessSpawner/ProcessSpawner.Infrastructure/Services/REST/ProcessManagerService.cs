using System;
using Authorization.Application.Services;
using AutoMapper;
using Microsoft.Extensions.Logging;
using ProcessSpawner.Application.DTOs;
using ProcessSpawner.Application.Repositories;
using ProcessSpawner.Application.Services.Common;
using ProcessSpawner.Application.Services.REST;
using ProcessSpawner.Domain.Models;
using ProcessSpawner.Infrastructure.Repositories;
using ProcessSpawner.Protobuf.Communication;
using Shared.Generic.RestApi;

namespace ProcessSpawner.Infrastructure.Services.REST {
    public class ProcessManagerService : IProcessManagerService {
        public readonly ILogger<ProcessManagerService> m_logger;
        public readonly IProcessManagerRepository m_processManagerRepository;
        private readonly IMapper m_mapper;

        public ProcessManagerService(ILogger<ProcessManagerService> logger, IProcessManagerRepository processManagerRepository, IMapper mapper) {
            m_logger = logger;
            m_processManagerRepository = processManagerRepository;
            m_mapper = mapper;
        }

        public async Task<ObjectOperationResult<ProcessManagerDto>> Create(ProcessManagerDto obj) {
            var objToCreate = m_mapper.Map<ProcessManager>(obj);
            var objRef = await m_processManagerRepository.AddAsync(objToCreate);

            return new ObjectOperationResult<ProcessManagerDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = m_mapper.Map<ProcessManagerDto>(objRef)
            };
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

        public async Task<ObjectOperationResult<ProcessManagerDto>> Put(int id, ProcessManagerDto obj) {
            var objToUpdate = await m_processManagerRepository.GetByIdAsync(id);
            if (objToUpdate == null) {
                throw new Exception($"Process manager with {id} doesn't exist");
            }

            objToUpdate.Name = obj.Name;
            objToUpdate.Port = obj.Port;
            objToUpdate.Address = obj.Address;

            m_processManagerRepository.Update(objToUpdate);

            return new ObjectOperationResult<ProcessManagerDto> {
                Status = BaseResponseStatus.Ok,
                ErrorMessage = string.Empty,
                Object = m_mapper.Map<ProcessManagerDto>(m_mapper.Map<ProcessManagerDto>(objToUpdate))
            };
        }
    }
}
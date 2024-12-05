using System;
namespace ProcessSpawner.Infrastructure.Tools {
    public class AutoMapperProfile : AutoMapper.Profile {
        public AutoMapperProfile() {
            CreateMap<Domain.Models.ProcessInstance, Application.DTOs.ProcessInstanceDto>();
            CreateMap<Application.DTOs.ProcessInstanceDto, Domain.Models.ProcessInstance>();
        }
    }
}
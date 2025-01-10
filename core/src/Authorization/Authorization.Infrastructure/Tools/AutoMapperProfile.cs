using System;

public class AutoMapperProfile : AutoMapper.Profile {
    public AutoMapperProfile() {
        CreateMap<Authorization.Domain.Models.User, Authorization.Application.DTOs.UserDto>();
        CreateMap<Authorization.Application.DTOs.UserDto, Authorization.Domain.Models.User>();
    }
}
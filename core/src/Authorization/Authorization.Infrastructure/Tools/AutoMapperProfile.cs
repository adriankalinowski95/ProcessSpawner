using System;

public class AutoMapperProfile : AutoMapper.Profile {
    public AutoMapperProfile() {
        CreateMap<Authorization.Domain.Models.User, Authorization.Application.DTOs.UserDto>();
    }
}
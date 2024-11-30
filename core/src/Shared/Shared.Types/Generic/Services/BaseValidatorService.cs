using System;
using FluentValidation;
using FluentValidation.Results;
using Shared.Generic.RestApi;

namespace Shared.Generic.Services {
    public class BaseValidatorService<T> {
        private IValidator<T> m_validator;

        public BaseValidatorService(IValidator<T> validator) {
            m_validator = validator;
        }

        public async Task<ObjectOperationResult<DTO>> SingleObjectValidation<DTO>(T obj) {
            ValidationResult validationResult = await m_validator.ValidateAsync(obj);
            if (validationResult.IsValid) {
                return null;
            }

            var errors = validationResult.Errors.Select(e => new BaseFieldValidationResponse {
                Field = e.PropertyName,
                Code = e.ErrorCode,
                Message = e.ErrorMessage
            }).ToList();

            return new ObjectOperationResult<DTO> {
                ValidationResult = errors,
                Status = BaseResponseStatus.Error,
                ErrorMessage = string.Empty
            };
        }
    }
}
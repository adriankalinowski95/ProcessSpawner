using System;
using Shared.Types.BaseClasses.Validators;

namespace Shared.Types.BaseClasses {
    public abstract class BaseValidationMethods {
        public abstract Task<bool> GetMethod<T>(BaseFieldUIDto field, BaseValidator Validator, T BotProfilSettingsRepository);
        /*
    {

        switch (Validator.ValidationFunctionName) {
            case "MaxLength":
                return await MaxLength(field, Validator, BotProfilSettingsRepository);
            case "MinLength":
                return await MinLength(field, Validator, BotProfilSettingsRepository);
            case "Required":
                return await Required(field, Validator, BotProfilSettingsRepository);
            case "ExistsInDataSource":
                return await ExistsInDataSource(field, Validator, (dynamic) BotProfilSettingsRepository);
            default:
                return await DefaultFunction(field, Validator, BotProfilSettingsRepository);
        }

    }
        */

        public async Task<bool> MaxLength<T>(BaseFieldUIDto field, BaseValidator Validator, T BotProfilSettingsRepository) {
            return field.Value.Length <= Validator.Value;
        }

        public async Task<bool> MinLength<T>(BaseFieldUIDto field, BaseValidator Validator, T BotProfilSettingsRepository) {
            return field.Value.Length >= Validator.Value;
        }

        public async Task<bool> ExistsInDataSource<T>(BaseFieldUIDto field, BaseValidator Validator, T service) {
            return false;
        }

        /*
        public async Task<bool> ExistsInDataSource(BaseFieldUIDto field, BaseValidator Validator, BotAlgorithmProfileHelperService service) {
            BotAlgorithmSettingsFieldDataSource ds = await service.GetSettingsFieldDataSource((int) field.SettingsFieldDataSourceId);

            return ds != null;
        }

        public async Task<bool> ExistsInDataSource(BaseFieldUIDto field, BaseValidator Validator, IBotProfileSettingsHelperService service) {
            BotProfileSettingsFieldDataSource ds = await service.GetSettingsFieldDataSource((int) field.SettingsFieldDataSourceId);

            return ds != null;
        }
        */

        public async Task<bool> Required<T>(BaseFieldUIDto field, BaseValidator Validator, T BotProfilSettingsRepository) {
            return !string.IsNullOrEmpty(field.Value);
        }

        public async Task<bool> DefaultFunction<T>(BaseFieldUIDto field, BaseValidator Validator, T BotProfilSettingsRepository) {
            return false;
        }
    }
}


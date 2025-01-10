using System;
namespace Shared.Types.BaseClasses {
    public class BaseFieldUIDto {
        public required string Key { get; set; }
        public required string Description { get; set; }
        public required string Value { get; set; }
        public bool IsPrivate { get; set; }
        public bool HasDataSource { get; set; }
        public Enums.BaseFieldUIType Type { get; set; }
        public int? SettingsFieldDataSourceId { get; set; }
    }
}
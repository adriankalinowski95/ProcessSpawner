using System;
namespace Shared.Types.BaseClasses {
    public class BaseFieldUIDto {
        public string Key { get; set; }
        public string Description { get; set; }
        public string Value { get; set; }
        public bool IsPrivate { get; set; }
        public bool HasDataSource { get; set; }
        public Enums.BaseFieldUIType Type { get; set; }
        public int? SettingsFieldDataSourceId { get; set; }
    }
}
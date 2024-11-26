using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Shared.Types.Enums;

namespace Shared.Types.BaseClasses.Validators {
    public class BaseValidator {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }
        public string Name { get; set; }
        public int? Value { get; set; }
        public string Description { get; set; }
        public ValidatorType? Type { get; set; }
        public BaseFieldUIType? FieldType { get; set; }
        public bool? isAsync { get; set; }
        public string ValidationMessage { get; set; }
        public string ValidationErrorCode { get; set; }
        public string ValidationFunctionName { get; set; }
        public bool IsGuiValidator { get; set; }
        public GuiType GuiType { get; set; }
    }
}
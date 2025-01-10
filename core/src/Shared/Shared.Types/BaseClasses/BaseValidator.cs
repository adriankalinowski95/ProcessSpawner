using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Shared.Types.Enums;

namespace Shared.Types.BaseClasses.Validators {
    public class BaseValidator {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }
        public required string Name { get; set; }
        public int? Value { get; set; }
        public required string Description { get; set; }
        public ValidatorType? Type { get; set; }
        public BaseFieldUIType? FieldType { get; set; }
        public bool? isAsync { get; set; }
        public required string ValidationMessage { get; set; }
        public required string ValidationErrorCode { get; set; }
        public required string ValidationFunctionName { get; set; }
        public bool IsGuiValidator { get; set; }
        public GuiType GuiType { get; set; }
    }
}
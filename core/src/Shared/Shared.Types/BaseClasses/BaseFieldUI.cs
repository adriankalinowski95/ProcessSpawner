using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Shared.Types.BaseClasses {
    public class BaseFieldUI {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }
        public required string Key { get; set; }
        public required string Description { get; set; }
        public string? Value { get; set; } = null;
        public bool IsPrivate { get; set; }
        public bool HasDataSource { get; set; }
        public Enums.BaseFieldUIType Type { get; set; }
    }
}
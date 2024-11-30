using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Microsoft.Extensions.Logging;

namespace Shared.Tools.Logging.Models {
    public class Log {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }
        public LogLevel Level { get; set; }
        public string CategoryName { get; set; }
        public string Message { get; set; }
        public DateTime CreateTime { get; set; } = DateTime.Now;
    }
}
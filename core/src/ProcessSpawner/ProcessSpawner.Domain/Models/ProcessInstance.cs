using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using ProcessSpawner.Domain.Enums;

namespace ProcessSpawner.Domain.Models {
    public class ProcessInstance {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }
        public string ProcessType { get; set; }
        public Dictionary<string, string> Parameters { get; set; }
        public string ProcessId { get; set; }
        public ProcessStatus Status { get; set; }
    }
}

using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using ProcessSpawner.Domain.Enums;

namespace ProcessSpawner.Domain.Models {
    public class ProcessInstance {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }
        public UInt32 ProcessId { get; set; }
        public string InternalId { get; set; }
        public string ProcessType { get; set; }
        public Dictionary<string, string> Parameters { get; set; }
        public ProcessStatus Status { get; set; }
        public DateTime CreatedTimeMs { get; set; }
        public DateTime LastUpdateTimeMs { get; set; }

        public int UserId { get; set; }

        public ProcessManager ProcessManager { get; set; }
        public int ProcessManagerId { get; set; }

        public List<ProcessEvent> ProcessEvents { get; set; } = new List<ProcessEvent>();
    }
}
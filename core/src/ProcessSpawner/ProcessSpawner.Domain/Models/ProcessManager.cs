using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ProcessSpawner.Domain.Models {
    public class ProcessManager {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }
        public string Name { get; set; }
        public string Address { get; set; }
        public UInt32 Port { get; set; }

        public List<ProcessInstance> ProcessInstances { get; set; } = new List<ProcessInstance>();
    }
}
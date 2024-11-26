using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ProcessSpawner.Domain.Models {
    public class ProcessEvent {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }
        public string Name { get; set; }
        public string Value { get; set; }
        public DateTime CreatedTimeMs { get; set; }

        public ProcessInstance ProcessInstance { get; set; }
        public int ProcessInstanceId { get; set; }
    }
}
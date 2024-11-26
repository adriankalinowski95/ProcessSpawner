using System;
namespace ProcessSpawner.Domain.Enums {
    public enum ProcessStatus {
        None,
        Started,
        Active,
        NonActive,
        Failed,
        Finished
    }
}
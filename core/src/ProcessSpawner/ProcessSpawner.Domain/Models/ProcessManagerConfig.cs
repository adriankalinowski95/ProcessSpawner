using System;
namespace ProcessSpawner.Domain.Models {
    public record ProcessManagerConfig(string address, uint port) {
        public string GetEndpoint() {
            return $"{address}:{port}";
        }
    }
}
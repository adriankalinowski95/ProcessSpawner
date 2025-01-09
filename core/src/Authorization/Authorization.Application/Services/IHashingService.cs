using System;

namespace Authorization.Application.Services {
    public interface IHashingService {
        public string HashString(string input);
        bool CompareStringWithHash(string input, string hash);
    }
}
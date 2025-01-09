using System;
using Authorization.Application.Services;
using System.Security.Cryptography;
using Microsoft.AspNetCore.Cryptography.KeyDerivation;

namespace Authorization.Infrastructure.Services {
    public class HashingService : IHashingService {
        private static byte[] Default_Salt = {
                0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8,
                0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10
        };

        private byte[] m_salt;

        public HashingService() {
            this.m_salt = Default_Salt;
        }

        public HashingService(byte[] salt) {
            this.m_salt = salt;
        }

        public string HashString(string input) {
            // byte[] salt = RandomNumberGenerator.GetBytes(128 / 8);
            string hashed = Convert.ToBase64String(
                KeyDerivation.Pbkdf2(
                    password: input,
                    salt: m_salt,
                    prf: KeyDerivationPrf.HMACSHA256,
                    iterationCount: 100000,
                    numBytesRequested: 256 / 8));

            return hashed;
        }

        public bool CompareStringWithHash(string input, string hash) {
            return HashString(input).CompareTo(hash) == 0;
        }
    }
}
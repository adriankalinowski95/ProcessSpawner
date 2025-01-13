using System;
using Newtonsoft.Json;
using System.Text;

namespace Shared.Tools.Utils {
    public static class Common {
        public static string GenerateRandomString(int length) {
            const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
            var random = new Random();
            var stringBuilder = new StringBuilder();

            for (int i = 0; i < length; i++) {
                stringBuilder.Append(chars[random.Next(chars.Length)]);
            }

            return stringBuilder.ToString();
        }

        public static T DeepClone<T>(T obj) {
            string json = JsonConvert.SerializeObject(obj);
            return JsonConvert.DeserializeObject<T>(json);
        }
    }
}


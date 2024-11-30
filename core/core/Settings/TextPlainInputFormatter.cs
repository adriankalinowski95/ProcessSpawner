using System;
using Microsoft.AspNetCore.Mvc.Formatters;
using System.Text;

namespace core.Settings {
    public class TextPlainInputFormatter : TextInputFormatter {
        public TextPlainInputFormatter() {
            SupportedMediaTypes.Add("text/plain");
            SupportedEncodings.Add(Encoding.UTF8);
            SupportedEncodings.Add(Encoding.Unicode);
        }

        protected override bool CanReadType(Type type) {
            return type == typeof(string);
        }

        public override async Task<InputFormatterResult> ReadRequestBodyAsync(InputFormatterContext context, Encoding encoding) {
            string data = null;
            using (var reader = new StreamReader(context.HttpContext.Request.Body, encoding)) {
                data = await reader.ReadToEndAsync();
            }

            return await InputFormatterResult.SuccessAsync(data);
        }
    }
}
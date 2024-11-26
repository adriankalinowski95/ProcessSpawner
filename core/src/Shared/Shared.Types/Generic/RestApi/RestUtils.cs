namespace Shared.Generic.RestApi {
    public class RestUtils {
        public static string FormatException(Exception e) {
            return string.Format("{0}{1}", e.Message, e.InnerException != null ? " (" + e.InnerException.Message + ")" : "");
        }
    }
}

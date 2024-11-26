namespace Shared.Generic.Services.String {
    public interface ICrudService<T> : IBaseCrudService<T, string> where T : class { }
}
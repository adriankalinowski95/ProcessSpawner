namespace Shared.Generic.Services.Int {
    public interface ICrudService<T> : IBaseCrudService<T, int> where T : class { }
    public interface ICrudService<Input, Output> : IBaseCrudService<Input, Output, int> { };
}
using System;
namespace Shared.Types.Tools {
    public interface IConverter<TSource, TDestination> {
        TDestination convert(TSource obj);
    }
}
#ifndef NONE_HPP_
#define NONE_HPP_

#include "capymeta/primitives/MetaFunctor.hpp"

namespace capy::meta
{

namespace hidden__
{
    struct Nullopt{};
}

template<typename T>
struct Maybe;

using None = Maybe<hidden__::Nullopt>;

template<typename T>
    requires (!std::same_as<hidden__::Nullopt, T>)
using Some = Maybe<T>;

template<typename T>
struct Maybe
{
    static constexpr bool HAS_VALUE = true;

    using Value = T;

    template<typename Mapper>
    using Map = Maybe<call_t<Mapper, T>>;

    template<typename Predicate>
    using Filter = std::conditional_t<
        call_v<Predicate, T>,
        Some<T>,
        None
    >;

    template<typename Mapper>
    using AndThen = call_t<Mapper, T>;

    template<typename OtherValue>
    using Or = Maybe<T>;
};

template<>
struct Maybe<hidden__::Nullopt>
{
    static constexpr bool HAS_VALUE = false;

    using Value = hidden__::Nullopt;

    template<typename Mapper>
    using Map = None;

    template<typename Predicate>
    using Filter = None;

    template<typename Mapper>
    using AndThen = None;

    template<typename OtherValue>
    using Or = Maybe<OtherValue>;
};

}

#endif // NONE_HPP_

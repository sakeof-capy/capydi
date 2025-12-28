#ifndef NONE_HPP_
#define NONE_HPP_

#include "capymeta/concepts/Trait.hpp"

namespace capy::meta
{

namespace implementation_details_
{
    struct Nullopt{};
}

template<typename T>
struct Maybe;

using None = Maybe<implementation_details_::Nullopt>;

template<typename T>
    requires (!std::same_as<implementation_details_::Nullopt, T>)
using Some = Maybe<T>;

template<typename T>
struct Maybe
{
    static constexpr bool HAS_VALUE = true;

    using Value = T;

    template<t_trait<MetaArity::N1> Mapper>
    using Map = Maybe<call_t<Mapper, T>>;

    template<v_trait<MetaArity::N1> Predicate>
    using Filter = std::conditional_t<
        call_v<Predicate, T>,
        Some<T>,
        None
    >;

    template<t_trait<MetaArity::N1> Mapper>
    using AndThen = call_t<Mapper, T>;

    template<typename OtherValue>
    using Or = Maybe<T>;
};

template<>
struct Maybe<implementation_details_::Nullopt>
{
    static constexpr bool HAS_VALUE = false;

    using Value = implementation_details_::Nullopt;

    template<t_trait<MetaArity::N1> Mapper>
    using Map = None;

    template<v_trait<MetaArity::N1> Predicate>
    using Filter = None;

    template<t_trait<MetaArity::N1> Mapper>
    using AndThen = None;

    template<typename OtherValue>
    using Or = Maybe<OtherValue>;
};

}

#endif // NONE_HPP_

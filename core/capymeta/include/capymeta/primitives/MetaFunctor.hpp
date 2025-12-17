#ifndef META_FUNCTOR_HPP_
#define META_FUNCTOR_HPP_

#include "capymeta/primitives/Pack.hpp"

#include <functional>

namespace capy::meta
{

template<auto Functor>
struct MetaUnary
{
    template<typename T>
    struct Call
    {
        using type = std::invoke_result_t<
            decltype(Functor),
            Unit<T>
        >;
    };
};

template<auto Functor>
struct UnaryPredicate
{
    template<typename T>
    struct Call
    {
        static constexpr bool value = std::invoke(
            Functor,
            Unit<T>{}
        );
    };
};

}

#endif // !META_FUNCTOR_HPP_

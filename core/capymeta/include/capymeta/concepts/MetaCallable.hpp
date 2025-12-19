#ifndef FUNCTOR_CONCEPTS_HPP_
#define FUNCTOR_CONCEPTS_HPP_

#include "capymeta/primitives/Pack.hpp"

#include <algorithm>

namespace capy::meta
{

enum class MetaCallableTag
{
    TYPE_CALLABLE,
    VALUE_CALLABLE,
};

namespace hidden__
{
    template<
        template<typename...> typename Callable
    >
    struct MetaCallable{};

    template<typename MetaFunctor_, typename... Args>
    using functor_call_t = MetaFunctor_::template Functor<Args...>;

    template<typename T, MetaCallableTag TAG>
    concept TaggedWith = 
        std::ranges::any_of(T::META_CALLABLE_TAGS, [](MetaCallableTag tag) {
            return tag == TAG;
        });
}

template<typename T>
concept MetaCallable = requires {
    typename hidden__::MetaCallable<T::template Functor>;
    T::META_CALLABLE_TAGS;
};

template<typename T>
concept TypeMetaCallable 
    =   MetaCallable<T>
    &&  hidden__::TaggedWith<T, MetaCallableTag::TYPE_CALLABLE>;

template<typename T>
concept ValueMetaCallable 
    =   MetaCallable<T>
    &&  hidden__::TaggedWith<T, MetaCallableTag::VALUE_CALLABLE>;

template<TypeMetaCallable MetaFunctor_, typename... Args>
using call_t = typename hidden__::functor_call_t<MetaFunctor_, Args...>::type;

template<ValueMetaCallable MetaFunctor_, typename... Args>
constexpr auto call_v = hidden__::functor_call_t<MetaFunctor_, Args...>::value;

}

#endif // !FUNCTOR_CONCEPTS_HPP_

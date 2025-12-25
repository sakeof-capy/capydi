#ifndef FUNCTOR_CONCEPTS_HPP_
#define FUNCTOR_CONCEPTS_HPP_

#include "capymeta/primitives/Pack.hpp"
#include "capymeta/primitives/MetaArity.hpp"

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
    concept tagged_with = 
        std::ranges::any_of(T::META_CALLABLE_TAGS, [](MetaCallableTag tag) {
            return tag == TAG;
        });

    template<typename T, MetaArity ARITY>
    concept of_arity 
        =   requires {
                T::META_CALLABLE_ARITIES;
            } 
        &&  std::ranges::any_of(T::META_CALLABLE_ARITIES, [](MetaArity arity) {
                return arity == ARITY;
            });
}

template<typename T>
concept MetaCallable = requires {
    typename hidden__::MetaCallable<T::template Functor>;
    T::META_CALLABLE_TAGS;
};

// TODO: maybe simplify by having MetaArity... only here by hiding
// the arities count requirement deeper into the hidden concept?
template<
    typename T, 
    MetaArity HEAD_ARITY,
    MetaArity... TAIL_ARITIES
>
concept t_trait
    =   MetaCallable<T>
    &&  hidden__::tagged_with<T, MetaCallableTag::TYPE_CALLABLE>
    &&  hidden__::of_arity<T, HEAD_ARITY>
    &&  (hidden__::of_arity<T, TAIL_ARITIES> && ...);

template<
    typename T, 
    MetaArity HEAD_ARITY,
    MetaArity... TAIL_ARITIES
>
concept v_trait 
    =   MetaCallable<T>
    &&  hidden__::tagged_with<T, MetaCallableTag::VALUE_CALLABLE>
    &&  hidden__::of_arity<T, HEAD_ARITY>
    &&  (hidden__::of_arity<T, TAIL_ARITIES> && ...);

template<typename MetaFunctor, typename... Args>
    requires t_trait<MetaFunctor, meta_arity_from_args_v<Args...>>
using call_t = typename hidden__::functor_call_t<MetaFunctor, Args...>::type;

template<typename MetaFunctor, typename... Args>
    requires v_trait<MetaFunctor, meta_arity_from_args_v<Args...>>
constexpr auto call_v = hidden__::functor_call_t<MetaFunctor, Args...>::value;

}

#endif // !FUNCTOR_CONCEPTS_HPP_

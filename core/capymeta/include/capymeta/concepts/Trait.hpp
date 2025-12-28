#ifndef FUNCTOR_CONCEPTS_HPP_
#define FUNCTOR_CONCEPTS_HPP_

#include "capymeta/primitives/MetaArity.hpp"
#include "capymeta/primitives/FixTemplateMetaArity.hpp"

#include <algorithm>

namespace capy::meta
{

enum class MetaCallableTag
{
    TYPE_CALLABLE,
    VALUE_CALLABLE,
};

namespace implementation_details_
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
    typename implementation_details_::MetaCallable<T::template Functor>;
    T::META_CALLABLE_TAGS;
};

template<
    typename T, 
    MetaArity HEAD_ARITY,
    MetaArity... TAIL_ARITIES
>
concept t_trait
    =   MetaCallable<T>
    &&  implementation_details_::tagged_with<T, MetaCallableTag::TYPE_CALLABLE>
    &&  implementation_details_::of_arity<T, HEAD_ARITY>
    &&  (implementation_details_::of_arity<T, TAIL_ARITIES> && ...);

template<
    typename T, 
    MetaArity HEAD_ARITY,
    MetaArity... TAIL_ARITIES
>
concept v_trait 
    =   MetaCallable<T>
    &&  implementation_details_::tagged_with<T, MetaCallableTag::VALUE_CALLABLE>
    &&  implementation_details_::of_arity<T, HEAD_ARITY>
    &&  (implementation_details_::of_arity<T, TAIL_ARITIES> && ...);

template<typename MetaFunctor, typename... Args>
    requires t_trait<MetaFunctor, meta_arity_from_args_v<Args...>>
using call_t = typename implementation_details_::functor_call_t<MetaFunctor, Args...>::type;

template<typename MetaFunctor, typename... Args>
    requires v_trait<MetaFunctor, meta_arity_from_args_v<Args...>>
constexpr auto call_v = implementation_details_::functor_call_t<MetaFunctor, Args...>::value;

template<typename MetaFunctor, MetaArity ARITY>
    requires t_trait<MetaFunctor, ARITY>
using as_nary_ft = fix_template_arity_ft<ARITY, MetaFunctor::template Functor>;

template<typename MetaFunctor, MetaArity ARITY>
    requires v_trait<MetaFunctor, ARITY>
using as_nary_fv = fix_template_arity_fv<ARITY, MetaFunctor::template Functor>;

template<typename MetaFunctor>
    requires t_trait<MetaFunctor, MetaArity::N1>
using as_unary_ft = as_nary_ft<MetaFunctor, MetaArity::N1>;

template<typename MetaFunctor>
    requires v_trait<MetaFunctor, MetaArity::N1>
using as_unary_fv = as_nary_fv<MetaFunctor, MetaArity::N1>;


}

#endif // !FUNCTOR_CONCEPTS_HPP_

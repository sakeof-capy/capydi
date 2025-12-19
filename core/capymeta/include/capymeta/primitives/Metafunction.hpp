#ifndef META_FUNCTOR_HPP_
#define META_FUNCTOR_HPP_

#include "capymeta/primitives/Pack.hpp"
#include "capymeta/concepts/MetaCallable.hpp"

#include <functional>
#include <concepts>
#include <cstddef>
#include <array>

namespace capy::meta
{

namespace hidden__
{
    template<std::size_t ARITY, typename MetaFunctor_>
    struct AsNary;
}

template<auto Functor_>
struct TypeMetafunction
{
private:
    using SelfType = TypeMetafunction<Functor_>;

public:
    using as_unary = typename hidden__::AsNary<1, SelfType>;

    template<std::size_t ARITY>
    using as_nary = typename hidden__::AsNary<ARITY, SelfType>;

public:
    static constexpr std::array META_CALLABLE_TAGS = { 
        MetaCallableTag::TYPE_CALLABLE 
    };

public:
    template<typename... Args>
        requires std::invocable<decltype(Functor_), Pack<Args...>>
    struct Functor
    {
    public:
        using ReturnType = std::invoke_result_t<decltype(Functor_), Pack<Args...>>;
        using type = ReturnType;
    };
};

template<auto Functor_>
struct ValueMetafunction
{
private:
    using SelfType = ValueMetafunction<Functor_>;

public:
    using as_unary = typename hidden__::AsNary<1, SelfType>;

    template<std::size_t ARITY>
    using as_nary = typename hidden__::AsNary<ARITY, SelfType>;

public:
    static constexpr std::array META_CALLABLE_TAGS = { 
        MetaCallableTag::TYPE_CALLABLE,
        MetaCallableTag::VALUE_CALLABLE
    };

public:
    template<typename... Args>
        requires std::invocable<decltype(Functor_), Pack<Args...>>
    struct Functor
    {
    public:
        using ReturnType = std::invoke_result_t<decltype(Functor_), Pack<Args...>>;
        using type = ReturnType;

    public:
        static constexpr ReturnType value = std::invoke(
            Functor_,
            Pack<Args...>{}
        );
    };
};

namespace hidden__
{
    template<std::size_t ARITY, typename MetaFunctor_>
    struct AsNary;

    template<typename MetaFunctor_>
    struct AsNary<1, MetaFunctor_>
    {
        template<typename T>
        using Functor = MetaFunctor_::template Functor<T>;
    };

    template<typename MetaFunctor_>
    struct AsNary<2, MetaFunctor_>
    {
        template<typename T1, typename T2>
        using Functor = MetaFunctor_::template Functor<T1, T2>;
    };

    template<typename MetaFunctor_>
    struct AsNary<3, MetaFunctor_>
    {
        template<typename T1, typename T2, typename T3>
        using Functor = MetaFunctor_::template Functor<T1, T2, T3>;
    };

    template<typename MetaFunctor_>
    struct AsNary<4, MetaFunctor_>
    {
        template<
            typename T1, 
            typename T2, 
            typename T3,
            typename T4
        >
        using Functor = MetaFunctor_::template Functor<
            T1, T2, 
            T3, T4
        >;
    };

    template<typename MetaFunctor_>
    struct AsNary<5, MetaFunctor_>
    {
        template<
            typename T1, 
            typename T2, 
            typename T3,
            typename T4,
            typename T5
        >
        using Functor = MetaFunctor_::template Functor<
            T1, T2, 
            T3, T4,
            T5
        >;
    };
      
    template<typename MetaFunctor_>
    struct AsNary<6, MetaFunctor_>
    {
        template<
            typename T1, 
            typename T2, 
            typename T3,
            typename T4,
            typename T5,
            typename T6
        >
        using Functor = MetaFunctor_::template Functor<
            T1, T2, 
            T3, T4,
            T5, T6
        >;
    };

    template<typename MetaFunctor_>
    struct AsNary<7, MetaFunctor_>
    {
        template<
            typename T1, 
            typename T2, 
            typename T3,
            typename T4,
            typename T5,
            typename T6,
            typename T7
        >
        using Functor = MetaFunctor_::template Functor<
            T1, T2, 
            T3, T4,
            T5, T6,
            T7
        >;
    };

    template<typename MetaFunctor_>
    struct AsNary<8, MetaFunctor_>
    {
        template<
            typename T1, 
            typename T2, 
            typename T3,
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8
        >
        using Functor = MetaFunctor_::template Functor<
            T1, T2, 
            T3, T4,
            T5, T6,
            T7, T8
        >;
    };
}

}

#endif // !META_FUNCTOR_HPP_

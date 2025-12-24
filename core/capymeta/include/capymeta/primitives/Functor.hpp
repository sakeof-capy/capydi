#ifndef META_FUNCTION_HPP_
#define META_FUNCTION_HPP_

#include "capymeta/concepts/MetaCallable.hpp"
#include "capymeta/primitives/FixTemplateArity.hpp"

#include <cstddef>
#include <array>
#include <functional>

namespace capy::meta
{

template<auto Functor_>
struct functor_ft
{
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
        using type = std::invoke_result_t<decltype(Functor_), Pack<Args...>>;
    };

public:
    template<TemplateArity ARITY>
    using as_nary = fix_template_arity_ft<ARITY, Functor>;

    using as_unary = as_nary<TemplateArity::N1>;
};

template<auto Functor_>
struct functor_fv
{
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
    private:
        using ReturnType = std::invoke_result_t<decltype(Functor_), Pack<Args...>>;
    
    public:
        using type = ReturnType;

    public:
        static constexpr ReturnType value = std::invoke(
            Functor_,
            Pack<Args...>{}
        );
    };

public:
    template<TemplateArity ARITY>
    using as_nary = fix_template_arity_fv<ARITY, Functor>;

    using as_unary = as_nary<TemplateArity::N1>;
};

}

#endif // !META_FUNCTION_HPP_

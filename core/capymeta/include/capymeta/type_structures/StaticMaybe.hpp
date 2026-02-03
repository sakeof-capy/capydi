#ifndef CAPYMETA_STATIC_MAYBE_HPP_
#define CAPYMETA_STATIC_MAYBE_HPP_

#include "Maybe.hpp"

#include <optional>

namespace capy::meta
{

namespace implementation_details_
{
    /* 
        We tend to use type tags instead of bools 
        to allow using wrapped_with concept for 
        return values that have StaticMaybe-ish type
    */
    struct SomeTag{};
    struct NoneTag{};
}

template<typename Value, typename Tag>
class StaticMaybe;

template<typename Value>
class StaticMaybe<Value, implementation_details_::SomeTag>
{
public:
    constexpr explicit StaticMaybe(Value&& value)
        : value_ { std::move(value) }
    {}

public:
    static consteval bool has_value()
    {
        return HAS_VALUE;
    }

    template<typename Self>
    constexpr decltype(auto) value(this Self&& self)
    {
        return std::forward<Self>(self).value_;
    }

    constexpr std::optional<Value> as_optional() && 
    {
        return std::optional { std::move(this->value_) };
    }

private:
    static constexpr bool HAS_VALUE = true;
    
private:
    Value value_;
};

template<typename Value>
class StaticMaybe<Value, implementation_details_::NoneTag>
{
public:
    static consteval bool has_value()
    {
        return HAS_VALUE;
    }

    template<typename Self>
    constexpr decltype(auto) value(this Self&& self)
    {
        static_assert(false, "Trying to retrieve value from null optional");
    }

    constexpr std::optional<Value> as_optional() && 
    {
        return std::nullopt;
    }

private:
    static constexpr bool HAS_VALUE = false;
};

template<typename Value>
StaticMaybe(Value&&) -> StaticMaybe<Value, implementation_details_::SomeTag>;

template<typename Value>
using StaticNone = StaticMaybe<Value, implementation_details_::NoneTag>;

}

#endif // CAPYMETA_STATIC_MAYBE_HPP_

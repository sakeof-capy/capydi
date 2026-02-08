#ifndef CAPYMETA_STATIC_EITHER_HPP_
#define CAPYMETA_STATIC_EITHER_HPP_

#include <expected>

namespace capy::meta
{

namespace implementation_details_
{
    /* 
        We tend to use type tags instead of bools 
        to allow using wrapped_with concept for 
        return values that have StaticEither-ish type
    */
    struct ValueTag{};
    struct ErrorTag{};
}

template<typename Value, typename Error, typename Tag>
class StaticEither;

template<typename Value, typename Error>
class StaticEither<Value, Error, implementation_details_::ValueTag>
{
public:
    using value_type = Value;
    using error_type = Error;
    
public:
    constexpr /* implicit */ StaticEither(Value&& value)
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

    template<typename Self>
    constexpr decltype(auto) error(this Self&& self)
    {
        static_assert(
            false, 
            "Trying to extract an error while the either-object contains value"
        );
    }

    constexpr std::expected<Value, Error> as_expected() && 
    {
        return std::expected { std::move(this->value_) };
    }

private:
    static constexpr bool HAS_VALUE = true;
    
private:
    Value value_;
};

template<typename Value, typename Error>
class StaticEither<Value, Error, implementation_details_::ErrorTag>
{
public:
    using value_type = Value;
    using error_type = Error;

public:
    constexpr /* implicit */ StaticEither(Error&& error)
        : error_ { std::move(error) }
    {}

public:
    static consteval bool has_value()
    {
        return HAS_VALUE;
    }

    template<typename Self>
    constexpr decltype(auto) value(this Self&& self)
    {
        static_assert(false, "Trying to retrieve value from error either-object");
    }

    template<typename Self>
    constexpr decltype(auto) error(this Self&& self)
    {
        return std::forward<Self>(self).error_;
    }

    constexpr std::expected<Value, Error> as_expected() && 
    {
        return std::unexpected { std::move(this->error_) };
    }

private:
    static constexpr bool HAS_VALUE = false;

private:
    Error error_;
};

template<typename Value, typename Error>
using StaticOk = StaticEither<Value, Error, implementation_details_::ValueTag>;

template<typename Value, typename Error>
using StaticError = StaticEither<Value, Error, implementation_details_::ErrorTag>;

}

#endif // !CAPYMETA_STATIC_EITHER_HPP_

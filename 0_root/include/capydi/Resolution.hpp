#ifndef RESOLUTION_HPP_
#define RESOLUTION_HPP_

#include "Error.hpp"
#include "utilities/referencing/Reference.hpp"

#include <type_traits>
#include <expected>

namespace capy::di
{

namespace hidden__
{
    template<typename T>
    struct IsWrappedWithExpected : std::false_type {};

    template<typename ValueType, typename ErrorType>
    struct IsWrappedWithExpected<
        std::expected<ValueType, ErrorType>
    >
        : std::true_type
    {};

    template<typename T>
    constexpr bool is_wrapped_with_expected = IsWrappedWithExpected<T>::value;
}

/**
 * aka std::expected<Reference<Dependency>, Error>
 */
template<typename T, typename Dependency>
concept Resolution 
    =   hidden__::is_wrapped_with_expected<T> 
    &&  Reference<typename T::value_type, Dependency> 
    &&  std::same_as<typename T::error_type, Error> ;

}

#endif // !RESOLUTION_HPP_

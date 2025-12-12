/**
 * @file Resolution.hpp
 * @brief Resolution concept and result type for dependency injection queries.
 * 
 * A resolution represents the successful resolution of a dependency: it is a reference
 * to an object of the requested type, wrapped in @c std::expected to handle errors gracefully.
 * 
 * The Resolution concept enforces that:
 * - The result type is @c std::expected<Reference<Dependency>, Error>
 * - The wrapped value is a valid reference to the requested dependency
 * - Errors are uniformly represented via the @c Error enum
 * 
 * @see Error
 * @see Reference
 */

#ifndef RESOLUTION_HPP_
#define RESOLUTION_HPP_

#include "utilities/referencing/Reference.hpp"

#include <type_traits>
#include <expected>

namespace capy::di
{

/// @cond HIDDEN

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

/// @endcond

/**
 * @concept Resolution
 * @brief Concept for a successful dependency resolution result.
 * 
 * @tparam T The result type being tested. Must be @c std::expected<Reference<Dependency>, Error_>.
 * @tparam Dependency The type of dependency being resolved.
 * @tparam Error_ The error type.
 * 
 * A type satisfies Resolution if:
 * - It is an instantiation of @c std::expected with a value type that satisfies @c Reference<Dependency>
 * - Its error type is exactly @c Error_
 * 
 * This design enables:
 * - **Compile-time verification** that resolution operations return the correct type
 * - **Zero-overhead abstraction**: std::expected has no runtime penalty if used carefully
 * - **Consistent error handling** across all DI operations
 * 
 * @example
 * @code
 * // This is a valid Resolution<MyService, CacheError>
 * auto result = container.resolve<ServiceKey>();  // std::expected<Reference<MyService>, Error>
 * 
 * if (result.has_value()) {
 *     auto& service = result.value().get();
 *     service.doWork();
 * } else {
 *     auto err = result.error();
 *     // Handle error...
 * }
 * @endcode
 */
template<typename T, typename Dependency, typename Error_>
concept Resolution 
    =   hidden__::is_wrapped_with_expected<T> 
    &&  Reference<typename T::value_type, Dependency> 
    &&  std::same_as<typename T::error_type, Error_>;

}

#endif // !RESOLUTION_HPP_

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

#include <capymeta/concepts/WrappedWIth.hpp>
#include <capymeta/type_structures/StaticEither.hpp>
#include <capymeta/primitives/referencing/Reference.hpp>
#include <type_traits>

namespace capy::di
{

template<typename T, typename Dependency, typename Error_>
concept Resolution 
    =   meta::wrapped_with<T, meta::StaticEither> 
    &&  meta::Reference<typename T::value_type, Dependency> 
    &&  std::same_as<typename T::error_type, Error_>;

}

#endif // !RESOLUTION_HPP_

/**
 * @file Error.hpp
 * @brief Error codes and diagnostics for dependency injection operations.
 * 
 * This header defines the error conditions that can occur during dependency resolution
 * and creation. All resolution operations return @c std::expected<Resolution, Error>
 * to enable elegant error handling without exceptions.
 * 
 * @see Resolution
 */

#ifndef ERROR_HPP_
#define ERROR_HPP_

namespace capy::di
{

/**
 * @enum Error
 * @brief Enumeration of possible errors during dependency injection resolution.
 * 
 * These errors are returned via @c std::expected to allow compile-time-aware error handling.
 * Users can inspect the error and decide whether to recover, provide a default, or propagate.
 */
enum class Error
{
    /// @brief A dependency cannot be resolved because no suitable config was found.
    CANNOT_BE_RESOLVED,
    
    /// @brief A dependency required by another dependency cannot be resolved.
    ///        This indicates a missing transitive dependency in the configuration graph.
    DEPENDENCY_CANNOT_BE_RESOLVED,

    CANNOT_DEDUCE_DEPENDENCIES_OF_TYPE,
};

}

#endif // !ERROR_HPP_

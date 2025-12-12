/**
 * @file KeyResolver.hpp
 * @brief Utility for mapping and resolving dependency keys across configurations.
 * 
 * The KeyResolver aggregates resolution keys from all configurations in the container,
 * enabling efficient type-based lookup of dependencies without runtime polymorphism.
 * This is a pure compile-time construct with zero runtime overhead.
 */

#ifndef KEY_RESOLVER_HPP_
#define KEY_RESOLVER_HPP_

#include "utilities/pack/Pack.hpp"
#include "utilities/pack/PackConcat.hpp"

namespace capy::di
{

/**
 * @class KeyResolver
 * @brief Aggregates and resolves dependency keys from a pack of configurations.
 * 
 * @tparam Configs Parameter pack of configuration types, each exposing a @c ResolutionKeysPack.
 * 
 * The KeyResolver combines the resolution keys from all input configurations into a single
 * @c CentralKeys pack, which can then be queried by the DI container to map requested keys
 * to the appropriate configuration handler.
 * 
 * This design supports:
 * - **Key aggregation**: Combining keys from heterogeneous config sources
 * - **Type-safe lookup**: Finding configs that handle a specific key at compile time
 * - **Zero-cost abstraction**: All operations are evaluated at compile time
 * 
 * @note This is primarily an internal utility; users typically don't interact with KeyResolver directly.
 */
template<typename... Configs>
struct KeyResolver
{
    /// Consolidated pack of all resolution keys from all configs
    using CentralKeys = pack_multy_concat_t<
        typename Configs::ResolutionKeysPack...
    >;
    
    /// Identity transformation: maps a key to itself (extensible for custom key transformations)
    template<typename Key>
    using ResolveKeys = Key;
};

}

#endif // !KEY_RESOLVER_HPP_

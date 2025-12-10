/**
 * @file ChainableConfig.hpp
 * @brief Concept and utilities for chainable (decorator/pipeline) configurations.
 * 
 * Chainable configurations form a transformation pipeline that is applied to created objects.
 * Common examples include caching layers, validation decorators, logging proxies, and 
 * security wrappers. The framework supports arbitrary chains of such decorators applied
 * in sequence after the initial object creation.
 */

#ifndef CHAINABLE_CONFIG_HPP_
#define CHAINABLE_CONFIG_HPP_

#include "utilities/referencing/Reference.hpp"
#include "configs/ConfigType.hpp"
#include <concepts>

namespace capy::di
{

/**
 * @concept ChainableConfig
 * @brief Concept for configuration objects that transform/decorate dependencies.
 * 
 * A ChainableConfig is a compile-time concept that ensures a type can participate
 * in the transformation pipeline. Each chainable config:
 * 
 * - Declares a @c RelatedEntity type (the type it transforms/decorates)
 * - Declares a @c RelatedKey type (used for lookup/matching in the chain)
 * - Implements a @c pipe() method that applies the transformation
 * - Is statically classified as @c ConfigType::CHAINABLE
 * 
 * @tparam Config The configuration type being tested.
 * 
 * @details The @c pipe() method signature (when enabled) should be:
 * @code
 * Reference<RelatedEntity> auto pipe(Reference<RelatedEntity> auto entity) const;
 * @endcode
 * 
 * This enables powerful decorator patterns like:
 * @code
 * class CacheDecorator {
 *     using RelatedEntity = MyService;
 *     using RelatedKey = CacheKey;
 *     static constexpr ConfigType CONFIG_TYPE = ConfigType::CHAINABLE;
 *     
 *     Reference<MyService> auto pipe(Reference<MyService> auto service) const {
 *         return cached_service(service);
 *     }
 * };
 * @endcode
 * 
 * @see ConfigType
 * @see CreationalConfig
 */
template<typename Config>
concept ChainableConfig = 
    requires() {
        typename Config::RelatedEntity;
        typename Config::RelatedKey;
        Config::CONFIG_TYPE;
    }
    // && requires(
    //     const Config& config 
    //     Reference<typename Config::RelatedEntity> auto related_entity
    // ) {
    //     { 
    //         config.pipe(related_entity) 
    //     } -> Reference<typename Config::RelatedEntity>;
    // }
    && std::same_as<std::remove_cv_t<decltype(Config::CONFIG_TYPE)>, ConfigType>
    && Config::CONFIG_TYPE == ConfigType::CHAINABLE;

/**
 * @struct IsChainableConfig
 * @brief Type trait that checks if a type is a ChainableConfig.
 * 
 * Useful for type filtering and pack operations.
 * 
 * @tparam Config The type to check.
 * @member value Boolean indicating whether @p Config satisfies ChainableConfig.
 */
template<typename Config>
struct IsChainableConfig
{
    static constexpr bool value = ChainableConfig<Config>;
};

/// Helper alias to extract the entity type from a ChainableConfig
template<ChainableConfig Config> 
using get_related_entity_t = typename Config::RelatedEntity;

/// Helper alias to extract the key type from a ChainableConfig
template<ChainableConfig Config> 
using get_related_key_t = typename Config::RelatedKey;

}

#endif // !CHAINABLE_CONFIG_HPP_

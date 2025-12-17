/**
 * @file CreationalConfig.hpp
 * @brief Concept and utilities for creational (factory/constructor) configurations.
 * 
 * Creational configurations handle object instantiation and creation patterns.
 * They are responsible for providing the initial instances that may then be
 * passed through chainable (decorator) configurations.
 */

#ifndef CREATIONAL_CONFIG_HPP_
#define CREATIONAL_CONFIG_HPP_

#include "capymeta/pack/Pack.hpp"
#include "capydi/referencing/Reference.hpp"

namespace capy::di
{

/**
 * @concept CreationalConfig
 * @brief Concept for configurations that handle dependency creation and instantiation.
 * 
 * A CreationalConfig declares:
 * - @c CentralType: The primary type being created/managed
 * - @c ResolutionKeysPack: A meta::Pack of types that can be used to look up this config
 * - @c CONFIG_TYPE: Must be @c ConfigType::CREATIONAL
 * - @c do_resolve(key): Method to perform the actual creation
 * 
 * @tparam Config The configuration type being tested.
 * 
 * @example
 * @code
 * class MyFactory {
 *     using CentralType = MyService;
 *     using ResolutionKeysPack = meta::Pack<ServiceKey, MyService>;
 *     static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;
 *     
 *     std::expected<RuntimeRef<MyService>, Error> do_resolve(ServiceKey key) const {
 *         return RuntimeRef(create_service(key));
 *     }
 * };
 * @endcode
 * 
 * @see ConfigType
 * @see ChainableConfig
 */
template<typename Config>
concept CreationalConfig = requires() {
    typename Config::CentralType;
    typename Config::ResolutionKeysPack;
    Config::CONFIG_TYPE;
}
&& std::same_as<std::remove_cv_t<decltype(Config::CONFIG_TYPE)>, ConfigType>
&& Config::CONFIG_TYPE == ConfigType::CREATIONAL;

/**
 * @struct IsCreationalConfig
 * @brief Type trait that checks if a type is a CreationalConfig.
 * 
 * Useful for type filtering and pack operations.
 * 
 * @tparam Config The type to check.
 * @member value Boolean indicating whether @p Config satisfies CreationalConfig.
 */
template<typename Config>
struct IsCreationalConfig
{
    static constexpr bool value = CreationalConfig<Config>;
};

/// Helper alias to extract the central type from a CreationalConfig
template<CreationalConfig Config>
using central_type_t = typename Config::CentralType;

/// Helper alias to extract the resolution keys from a CreationalConfig
template<CreationalConfig Config>
using resolution_keys_pack_t = typename Config::ResolutionKeysPack;

}  

#endif // !CREATIONAL_CONFIG_HPP_

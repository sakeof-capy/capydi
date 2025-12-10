/**
 * @file ConfigType.hpp
 * @brief Configuration type enumeration for the dependency injection system.
 * 
 * Configurations in Capydi are classified into three categories based on their role
 * in the dependency resolution pipeline. This classification drives the internal
 * dispatcher selection and resolution strategy.
 */

#ifndef CONFIG_TYPE_HPP_
#define CONFIG_TYPE_HPP_

namespace capy::di
{

/**
 * @enum ConfigType
 * @brief Categorization of configuration strategies in the DI container.
 * 
 * Each configuration must declare its type to enable the container to route it
 * to the appropriate dispatcher and resolution strategy.
 */
enum class ConfigType : unsigned char
{
    /// @brief Creational configs handle object instantiation and factory patterns.
    ///        These are responsible for creating instances of requested types.
    CREATIONAL,
    
    /// @brief Decorative configs wrap or augment created objects.
    ///        (Alternative/deprecated term for CHAINABLE)
    DECORATIVE,
    
    /// @brief Chainable configs form a pipeline of transformations applied after creation.
    ///        Examples: proxies, interceptors, validators, caches.
    CHAINABLE,

    /// @brief Sentinel value for bounds checking and iteration.
    SIZE
};

}

#endif // !CONFIG_TYPE_HPP_

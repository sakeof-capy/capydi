/**
 * @file DiConfig.hpp
 * @brief Master concept combining all valid DI configurations.
 * 
 * DiConfig is the top-level concept that encompasses all valid configurations
 * for the DI container. Any configuration satisfying either CreationalConfig
 * or ChainableConfig is a valid DiConfig.
 */

#ifndef DI_CONFIG_HPP_
#define DI_CONFIG_HPP_

#include "ChainableConfig.hpp"
#include "CreationalConfig.hpp"

namespace capy::di
{ 

/**
 * @concept DiConfig
 * @brief Master concept for all valid DI configurations.
 * 
 * @tparam Config The configuration type being tested.
 * 
 * A type satisfies DiConfig if it satisfies either:
 * - @c CreationalConfig: Handles object creation
 * - @c ChainableConfig: Handles object decoration/transformation
 * 
 * This is the concept used by the DI container to accept and validate configurations.
 * 
 * @see CreationalConfig
 * @see ChainableConfig
 */
template<typename Config>
concept DiConfig = CreationalConfig<Config> || ChainableConfig<Config>;

}

#endif // !DI_CONFIG_HPP_

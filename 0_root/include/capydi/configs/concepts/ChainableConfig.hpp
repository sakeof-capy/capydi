#ifndef CHAINABLE_CONFIG_HPP_
#define CHAINABLE_CONFIG_HPP_

#include "utilities/referencing/Reference.hpp"
#include "configs/ConfigType.hpp"
#include <concepts>

namespace capy::di
{

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

template<typename Config>
struct IsChainableConfig
{
    static constexpr bool value = ChainableConfig<Config>;
};

template<ChainableConfig Config> 
using get_related_entity_t = typename Config::RelatedEntity;

template<ChainableConfig Config> 
using get_related_key_t = typename Config::RelatedKey;

}

#endif // !CHAINABLE_CONFIG_HPP_

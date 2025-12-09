#ifndef CREATIONAL_CONFIG_HPP_
#define CREATIONAL_CONFIG_HPP_

#include "utilities/pack/Pack.hpp"
#include "utilities/referencing/Reference.hpp"

namespace capy::di
{

template<typename Config>
concept CreationalConfig = requires() {
    typename Config::CentralType;
    typename Config::ResolutionKeysPack;
    Config::CONFIG_TYPE;
}
&& std::same_as<std::remove_cv_t<decltype(Config::CONFIG_TYPE)>, ConfigType>
&& Config::CONFIG_TYPE == ConfigType::CREATIONAL;

template<typename Config>
struct IsCreationalConfig
{
    static constexpr bool value = CreationalConfig<Config>;
};

template<CreationalConfig Config>
using central_type_t = typename Config::CentralType;

template<CreationalConfig Config>
using resolution_keys_pack_t = typename Config::ResolutionKeysPack;

}

#endif // !CREATIONAL_CONFIG_HPP_

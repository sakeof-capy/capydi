#ifndef DI_CONFIG_HPP_
#define DI_CONFIG_HPP_

#include "ChainableConfig.hpp"
#include "CreationalConfig.hpp"

namespace capy::di
{

template<typename Config>
concept DiConfig = CreationalConfig<Config> || ChainableConfig<Config>;

}

#endif // !DI_CONFIG_HPP_

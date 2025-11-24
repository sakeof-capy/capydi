#ifndef DECORATABLE_CONFIG_HPP_
#define DECORATABLE_CONFIG_HPP_

namespace capy::di
{

template<typename Self>
struct DecoratableConfig
{
    template<typename DecorativeConfig>
    using with = DecorativeConfig::template Decorator<Self>;
};

}

#endif // !DECORATABLE_CONFIG_HPP_
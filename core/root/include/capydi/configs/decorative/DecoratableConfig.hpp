#ifndef DECORATABLE_CONFIG_HPP_
#define DECORATABLE_CONFIG_HPP_

namespace capy::di
{

template<typename Self>
struct DecoratableConfig
{
    template<typename Decorator, typename... Args>
    auto with(Args&&... args) &&
    {
        return Decorator::decorate(
            std::move(static_cast<Self&>(*this)),
            std::forward<Args>(args)...
        );
    }
};

template<typename Self>
struct DecoratableChainableConfig
{
    template<typename Decorator, typename... Args>
    auto with(Args&&... args) &&
    {
        return Decorator::decorate_chainable(
            std::move(static_cast<Self&>(*this)),
            std::forward<Args>(args)...
        );
    }
};

}

#endif // !DECORATABLE_CONFIG_HPP_
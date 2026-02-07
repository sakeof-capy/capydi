#ifndef DECORATOR_HPP_
#define DECORATOR_HPP_

#include "capydi/configs/concepts/ChainableConfig.hpp"
#include "capydi/configs/creational/Singleton.hpp"

#include <capymeta/primitives/referencing/Reference.hpp>
#include <type_traits>

namespace capy::di
{

template<
    typename Decorator_,
    typename Decoratee, 
    typename RelatedKey_ = meta::Pack<Decoratee>, 
    typename InnerConfig = Singleton<Decorator_>
>
struct Decorator
{
public:
    using RelatedEntity = Decoratee;
    using RelatedKey = RelatedKey_;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CHAINABLE;

public:
    meta::Reference<RelatedEntity> auto
        pipe(
            meta::Reference<RelatedEntity> auto decoratee
        ) const
    {
        auto dependencies = std::tuple { decoratee };
        Decorator_& decorator_ref = inner_config_.do_resolve(
            meta::Pack<Decorator_>{}, 
            dependencies
        );
        return meta::RuntimeRef (static_cast<RelatedEntity&>(decorator_ref));
    }

private:
    InnerConfig inner_config_;
};

}

#endif // !DECORATOR_HPP_

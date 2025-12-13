#ifndef DECORATOR_HPP_
#define DECORATOR_HPP_

#include "capydi/configs/concepts/ChainableConfig.hpp"
#include "capydi/utilities/referencing/Reference.hpp"
#include "capydi/configs/creational/Singleton.hpp"

#include <type_traits>

namespace capy::di
{

template<
    typename Decorator_,
    typename Decoratee, 
    typename RelatedKey_ = Pack<Decoratee>, 
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
    Reference<RelatedEntity> auto
        pipe(
            Reference<RelatedEntity> auto decoratee
        ) const
    {
        auto dependencies = std::tuple { decoratee };
        Decorator_& decorator_ref = inner_config_.do_resolve(
            Pack<Decorator_>{}, 
            dependencies
        );
        return RuntimeRef (static_cast<RelatedEntity&>(decorator_ref));
    }

private:
    InnerConfig inner_config_;
};

}

#endif // !DECORATOR_HPP_

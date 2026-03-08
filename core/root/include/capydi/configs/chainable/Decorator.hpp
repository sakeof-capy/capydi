#ifndef DECORATOR_HPP_
#define DECORATOR_HPP_

#include "capydi/configs/creational/Transient.hpp"
#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/Error.hpp"

#include <capymeta/primitives/referencing/Reference.hpp>
#include <capymeta/primitives/Pack.hpp>
#include <capymeta/type_structures/Maybe.hpp>
#include <capymeta/concepts/WrappedWIth.hpp>

namespace capy::di
{

template<
    typename Decorator_,
    typename Decoratee, 
    typename RelatedKey_ = meta::Pack<Decoratee>, 
    typename InnerConfig = Transient<Decorator_>
>
struct Decorator
    : public DecoratableChainableConfig<
        Decorator<Decorator_, Decoratee, RelatedKey_, InnerConfig>
    >
{
public:
    using RelatedEntity = Decoratee;
    using RelatedKeysPack = meta::Pack<RelatedKey_>;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CHAINABLE;

public:
    Resolution<RelatedEntity, Error> auto
        pipe(
            meta::Reference<RelatedEntity> auto decoratee,
            meta::wrapped_with<ResolutionContext> auto& context
        ) const
    {
        auto dependencies = std::tuple { decoratee };

        return inner_config_
            .do_resolve(meta::Pack<Decorator_>{}, dependencies, context, std::tuple{})
            .transform([](auto decorator_ref) {
                return meta::RuntimeRef (static_cast<RelatedEntity&>(decorator_ref));
            });
    }

private:
    InnerConfig inner_config_;
};

}

#endif // !DECORATOR_HPP_

#ifndef CAPYDI_ON_OBJECT_CREATED_HPP_
#define CAPYDI_ON_OBJECT_CREATED_HPP_

#include "capydi/Resolution.hpp"
#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/ResolutionContext.hpp"
#include "capydi/Error.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/concepts/WrappedWIth.hpp>
#include <capymeta/algorithms/pack/legacy/FunctionTraits.hpp>
#include <functional>

namespace capy::di
{

template<typename Type>
class DynamicResolver
{
public:
    using ReturnType = std::expected<meta::RuntimeRef<Type>, Error>;

public:
    explicit DynamicResolver(
        meta::wrapped_with<ResolutionContext> auto const& context
    )
        : resolver_ { [context] {
            return context.container.template resolve<Type>();
        }}
    {}

public:
    ReturnType resolve() const
    {
        return this->resolver_();
    }

private:
    std::function<ReturnType()> resolver_;
};

template<typename ActionArg>
struct ActionArgMatcher;


template<typename Type>
struct ActionArgMatcher<DynamicResolver<Type>>
{
    static std::expected<DynamicResolver<Type>, Error> prepare_arg(
        meta::wrapped_with<ResolutionContext> auto const& context
    ) {
        return DynamicResolver<Type> { context };
    }
};

template<typename Type>
struct ActionArgMatcher<Type&>
{
    static std::expected<meta::RuntimeRef<Type>, Error> prepare_arg(
        meta::wrapped_with<ResolutionContext> auto& context
    ) {
        return context.container.template resolve<Type>();
    }
};

template<
    typename RelatedEntity_,
    typename Action
>
class OnObjectCreated 
    : public DecoratableChainableConfig<
        OnObjectCreated<RelatedEntity_, Action>
    >
{
public:
    using RelatedEntity = RelatedEntity_;
    using RelatedKeysPack = meta::Pack<RelatedEntity_>;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CHAINABLE;

public:
    constexpr explicit OnObjectCreated(
        meta::Unit<RelatedEntity_>,
        Action&& action
    )
        : action_ { std::move(action) } 
    {}

public:
    Resolution<RelatedEntity, Error> auto
        pipe(
            meta::Reference<RelatedEntity> auto decoratee,
            meta::wrapped_with<ResolutionContext> auto& context
        ) const
    {
        if (!context.flags.just_created)
        {
            return std::expected<RelatedEntity, Error> {
                decoratee
            };
        }

        using ActionDependenciesPack 
            = meta::args_pack_t<decltype(Action::create)>;

        auto maybe_args = [&context]<typename... ActionDependencies>(meta::Pack<ActionDependencies...>) {
            std::tuple<std::expected<ActionDependencies, Error>...> args_preparations = {
                ActionArgMatcher<ActionDependencies>::prepare_arg(context)...
            };

            return std::apply([](auto&&... preparations) {
                using TupleResult = std::tuple<ActionDependencies...>;

                if ((preparations.has_value() && ...))
                {
                    return std::expected<TupleResult, Error> {
                        std::tuple { std::move(preparations.value())... }
                    };
                }

                return std::expected<TupleResult, Error> {
                    std::unexpected { Error::OBSERVER_COULD_NOT_RESOLVE_TYPE }
                };
            }, std::move(args_preparations));
        }(ActionDependenciesPack{});

        if (!maybe_args.has_value()) [[unlikely]]
        {
            return std::expected<RelatedEntity, Error> {
                std::unexpected { maybe_args.error() }
            };
        }

        auto args = maybe_args.value();
        
        std::apply(this->action_, args);

        return std::expected<RelatedEntity, Error> {
            decoratee
        };
    }

private:
    Action action_;
};

}

#endif // !CAPYDI_ON_OBJECT_CREATED_HPP_

#ifndef CAPYDI_ON_OBJECT_CREATED_HPP_
#define CAPYDI_ON_OBJECT_CREATED_HPP_

#include "capydi/Resolution.hpp"
#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/configs/decorative/DependencyTags.hpp"
#include "capydi/ResolutionContext.hpp"
#include "capydi/Error.hpp"
#include "capydi/configs/inputs/TagInput.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/concepts/WrappedWIth.hpp>
#include <capymeta/algorithms/pack/legacy/FunctionTraits.hpp>
#include <functional>
#include <optional>

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
        : resolver_ { [&context] {
            return context
                .container
                .template resolve<Type>();
        }}
    {}

    DynamicResolver(
        meta::wrapped_with<ResolutionContext> auto const& context,
        tag_t tag
    )
        : resolver_ { [context, tag] {
            return context
                .container
                .template resolve<Type>(std::tuple { TagInput {
                    tag
                }});
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
struct ActionArgMatcher<Type&>
{
    static std::expected<meta::RuntimeRef<Type>, Error> prepare_arg(
        meta::wrapped_with<ResolutionContext> auto& context,
        std::optional<tag_t> tag
    ) {
        if (tag.has_value())
        {
            auto resolution = context
                .container
                .template resolve<Type>(std::tuple { TagInput {
                    tag.value()
                }});

            return resolution;
        }
        else
        {
            auto resolution = context
                .container
                .template resolve<Type>();

            return resolution;
        }
    }
};

template<typename Type>
struct ActionArgMatcher<DynamicResolver<Type>>
{
    static std::expected<DynamicResolver<Type>, Error> prepare_arg(
        meta::wrapped_with<ResolutionContext> auto& context,
        std::optional<tag_t> tag
    ) {

        if (tag.has_value())
        {
            return DynamicResolver<Type> { 
                context,
                tag.value()
            };
        }
        else
        {
            return DynamicResolver<Type> { 
                context,
            }; 
        }
    }
};

template<
    typename RelatedEntity_,
    typename Action,
    std::size_t SIZE
>
class OnObjectCreated 
    : public DecoratableChainableConfig<
        OnObjectCreated<RelatedEntity_, Action, SIZE>
    >
{
public:
    using RelatedEntity = RelatedEntity_;
    using RelatedKeysPack = meta::Pack<meta::Pack<RelatedEntity_>>;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CHAINABLE;

public:
    constexpr explicit OnObjectCreated(
        meta::Unit<RelatedEntity_>,
        Action&& action,
        std::array<DependencyTagPair, SIZE> dependency_tags
    )
        : action_ { std::move(action) } 
        , dependency_tags_ { dependency_tags }
    {}

public:
    Resolution<RelatedEntity, Error> auto
        pipe(
            meta::Reference<RelatedEntity> auto decoratee,
            meta::wrapped_with<ResolutionContext> auto& context
        ) const
    {
        if (!context.flags.just_created) [[unlikely]]
        {
            return std::expected<decltype(decoratee), Error> {
                decoratee
            };
        }

        using ActionDependenciesPack 
            = meta::args_pack_t<decltype(&Action::operator())>;

        auto maybe_args = [this, &context]<typename... ActionDependencies>(
            meta::Pack<ActionDependencies...>
        ) {
            auto args_preparations = [this, &context]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                return std::tuple {
                    ActionArgMatcher<ActionDependencies>::prepare_arg(
                        context, 
                        [this]() -> std::optional<tag_t> {
                            auto it = std::ranges::find_if(
                                this->dependency_tags_, 
                                [](const auto& tag_pair){
                                    return tag_pair.first == Idx;
                                }
                            );

                            if (it == std::ranges::end(this->dependency_tags_)) [[unlikely]]
                            {
                                return std::nullopt;
                            }

                            return it->second;
                        }()
                    )...
                };
            }(std::index_sequence_for<ActionDependencies...>{});

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
            return std::expected<decltype(decoratee), Error> {
                std::unexpected { maybe_args.error() }
            };
        }

        auto& args = maybe_args.value();

        std::apply(this->action_, args);

        return std::expected<decltype(decoratee), Error> {
            decoratee
        };
    }

private:
    Action action_;
    std::array<DependencyTagPair, SIZE> dependency_tags_;
};

}

#endif // !CAPYDI_ON_OBJECT_CREATED_HPP_

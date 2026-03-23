#ifndef DECORATOR_HPP_
#define DECORATOR_HPP_

#include "capydi/configs/creational/Transient.hpp"
#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/configs/decorative/DependencyTags.hpp"
#include "capydi/configs/inputs/TagInput.hpp"
#include "capydi/Error.hpp"

#include <capymeta/primitives/referencing/RuntimeRef.hpp>
#include <capymeta/primitives/referencing/Reference.hpp>
#include <capymeta/primitives/Pack.hpp>
#include <capymeta/algorithms/pack/Head.hpp>
#include <capymeta/algorithms/pack/PopHead.hpp>
#include <capymeta/type_structures/Maybe.hpp>
#include <capymeta/concepts/WrappedWIth.hpp>

namespace capy::di
{

template<
    typename Decorator_,
    typename Decoratee, 
    std::size_t SIZE = 0
>
struct Decorator
    : public DecoratableChainableConfig<
        Decorator<Decorator_, Decoratee, SIZE>
    >
{
public:
    using RelatedEntity = Decoratee;
    using RelatedKeysPack = meta::Pack<meta::Pack<Decoratee>>;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CHAINABLE;

public:
    explicit Decorator(std::array<DependencyTagPair, SIZE> dependency_tags = {})
        : inner_config_{}
        , dependency_tags_ { dependency_tags }
    {}

    Decorator(
        meta::Unit<Decorator_>,
        meta::Unit<Decoratee>,
        std::array<DependencyTagPair, SIZE> dependency_tags = {}
    )
        : inner_config_{}
        , dependency_tags_ { dependency_tags }
    {}

public:
    Resolution<RelatedEntity, Error> auto
        pipe(
            meta::Reference<RelatedEntity> auto decoratee,
            meta::wrapped_with<ResolutionContext> auto& context
        ) const
    {
        using DependenciesPack = meta::args_pack_t<decltype(Decorator_::create)>;

        if constexpr (
            meta::pack_is_empty_v<DependenciesPack> ||
            !std::same_as<
                std::decay_t<meta::pack_head_t<DependenciesPack>>,
                std::decay_t<Decoratee> 
            >
        ) 
        {
            return std::expected<meta::RuntimeRef<RelatedEntity>, Error> {
                std::unexpected { Error::INVALID_DECORATOR_CONFIG }
            };
        }
        else 
        {
            using NonDecoratedDependenciesPack 
                = meta::pack_pop_head_t<DependenciesPack>;

            auto decorated_dependencies = std::tuple { decoratee };

            auto maybe_non_decorated_dependencies_tuple = [this, &context]<
                typename... NonDecoratedDependencies
            >(
                meta::Pack<NonDecoratedDependencies...>
            ) {
                return [this, &context]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                    return std::tuple { 
                        [this, &context] {
                            auto it = std::ranges::find_if(
                                this->dependency_tags, 
                                [](const auto& tag_pair){
                                    return tag_pair.first == Idx;
                                }
                            );

                            if (it == std::ranges::end(this->dependency_tags)) [[unlikely]]
                            {
                                return context
                                    .container
                                    .template resolve<NonDecoratedDependencies>();
                            }

                            return context
                                .container
                                .template resolve<NonDecoratedDependencies>(std::tuple { TagInput {
                                    it->second
                                }});
                        }()...
                    };
                }(std::index_sequence_for<NonDecoratedDependencies...>{});
            }(NonDecoratedDependenciesPack{});

            auto maybe_non_decorated_dependencies = std::apply(
                [](auto&&... maybe_dependencies) {
                    using DependenciesTuple = std::tuple<
                        typename std::remove_reference_t<decltype(maybe_dependencies)>::value_type...
                    >;

                    if ((maybe_dependencies.has_value() && ...))
                    {
                        return std::expected<DependenciesTuple, Error> {
                            std::tuple { std::move(maybe_dependencies.value())... }
                        };
                    }
                    else 
                    {
                        return std::expected<DependenciesTuple, Error> {
                            std::unexpected { Error::INVALID_DECORATOR_DEPENDENCIES }
                        };
                    }
                },
                std::move(maybe_non_decorated_dependencies_tuple)
            );

            if (!maybe_non_decorated_dependencies.has_value()) [[unlikely]]
            {
                return std::expected<meta::RuntimeRef<RelatedEntity>, Error> {
                    std::unexpected { maybe_non_decorated_dependencies.error() }
                };
            }

            auto non_decorated_dependencies 
                = std::move(maybe_non_decorated_dependencies).value();
            
            auto dependencies = std::tuple_cat(
                std::move(decorated_dependencies),
                std::move(non_decorated_dependencies)
            );

            return inner_config_
                .do_resolve(meta::Pack<Decorator_>{}, dependencies, context, std::tuple{})
                .transform([](auto decorator_ref) {
                    return meta::RuntimeRef (static_cast<RelatedEntity&>(decorator_ref));
                });
        }
    }

private:
    Transient<Decorator_> inner_config_;
    std::array<DependencyTagPair, SIZE> dependency_tags_;
};

}

#endif // !DECORATOR_HPP_

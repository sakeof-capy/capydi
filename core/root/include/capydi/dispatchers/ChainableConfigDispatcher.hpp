/**
 * @file ChainableConfigDispatcher.hpp
 * @brief Dispatcher that applies chainable (decorator) configurations to created objects.
 * 
 * The ChainableConfigDispatcher handles the second phase of dependency resolution:
 * applying a pipeline of transformations/decorators to created objects. It routes
 * piping requests to applicable ChainableConfigs based on their RelatedKey.
 * 
 * @see ChainableConfig
 * @see DI
 */

#ifndef CHAINABLE_CONFIG_DISPATCHER_HPP_
#define CHAINABLE_CONFIG_DISPATCHER_HPP_

#include "capydi/configs/concepts/ChainableConfig.hpp"
#include "capydi/Resolution.hpp"
#include "capydi/Error.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/primitives/referencing/Reference.hpp>
#include <capymeta/primitives/Template.hpp>
#include <capymeta/algorithms/pack/Filter.hpp>
#include <concepts>
#include <expected>

namespace capy::di
{

/// @cond IMPLEMENTATION

template<ChainableConfig... Configs>
class ChainableConfigDispatcher : private Configs...
{
private:
    using Configs::pipe...;

private:
    using ConfigsPack = meta::Pack<Configs...>;

    template<typename RelatedKey>
    struct RelatedConfigPredicate
    {
        template<typename Config>
        struct Predicate
        {
            static constexpr bool value = std::same_as<
                get_related_key_t<Config>,
                RelatedKey
            >;
        };
    };

    template<typename RelatedKey>
    using related_configs_pack_t = meta::pack_filter_t<
        ConfigsPack, 
        meta::template_fv<
            RelatedConfigPredicate<RelatedKey>::template Predicate,
            meta::MetaArity::N1
        >
    >;

public:
    constexpr explicit ChainableConfigDispatcher(Configs&&... configs) 
        : Configs(std::move(configs))...
    {}

    /// @brief Apply the decorator pipeline for a specific key to an entity
    template<typename RelatedKey, typename RelatedEntity>
    [[nodiscard]] constexpr Resolution<
        RelatedEntity, 
        Error
    > auto
        apply_configs_chain(
            meta::Reference<RelatedEntity> auto entity
        ) const 
    {
        using RelatedConfigsPack = related_configs_pack_t<RelatedKey>;
        
        #define RESOLUTION_ATTEMPT      \
            this->perform_piping(       \
                meta::Pack<RelatedEntity>{},   \
                RelatedConfigsPack{},   \
                entity                  \
            )

        //if constexpr (requires { RESOLUTION_ATTEMPT; })
        //{
            auto resolution = RESOLUTION_ATTEMPT;
            return std::expected<decltype(resolution), Error> { resolution };
        //}

        #undef RESOLUTION_ATTEMPT
        
        return std::expected<decltype(entity), Error> { entity };

    }
    
private:
    template<
        typename RelatedEntity, 
        typename HeadConfig, 
        typename... TailConfigs
    >
    [[nodiscard]] constexpr meta::Reference<RelatedEntity> auto
        perform_piping(
            meta::Pack<RelatedEntity>&&,
            meta::Pack<HeadConfig, TailConfigs...>&&, 
            meta::Reference<RelatedEntity> auto entity
        ) const 
    {
        const HeadConfig& 
            config = static_cast<const HeadConfig&>(*this);
        
        meta::Reference<RelatedEntity> auto 
            processed_entity = config.pipe(entity);

        return this->perform_piping(
            meta::Pack<RelatedEntity>{},
            meta::Pack<TailConfigs...>{}, 
            processed_entity
        );
    }

    template<
        typename RelatedEntity
    >
    [[nodiscard]] constexpr meta::Reference<RelatedEntity> auto
        perform_piping(
            meta::Pack<RelatedEntity>&&,
            meta::Pack<>&&, 
            meta::Reference<RelatedEntity> auto entity
        ) const 
    {
        return entity;
    }
};

}

#endif // !CHAINABLE_CONFIG_DISPATCHER_HPP_

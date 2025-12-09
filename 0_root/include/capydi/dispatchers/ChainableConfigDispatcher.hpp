#ifndef CHAINABLE_CONFIG_DISPATCHER_HPP_
#define CHAINABLE_CONFIG_DISPATCHER_HPP_

#include "configs/concepts/ChainableConfig.hpp"
#include "utilities/referencing/Reference.hpp"
#include "utilities/pack/Pack.hpp"
#include "utilities/pack/Filter.hpp"
#include "Resolution.hpp"
#include "Error.hpp"

#include <concepts>
#include <expected>

namespace capy::di
{

template<ChainableConfig... Configs>
class ChainableConfigDispatcher : private Configs...
{
private:
    using Configs::pipe...;

private:
    using ConfigsPack = Pack<Configs...>;

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
    using related_configs_pack_t = filter_t<
        ConfigsPack, 
        RelatedConfigPredicate<RelatedKey>::template Predicate
    >;

public:
    constexpr explicit ChainableConfigDispatcher(Configs&&... configs) 
        : Configs(std::move(configs))...
    {}

    template<typename RelatedKey, typename RelatedEntity>
    [[nodiscard]] constexpr Resolution<
        RelatedEntity, 
        Error
    > auto
        apply_configs_chain(
            Reference<RelatedEntity> auto entity
        ) const 
    {
        using RelatedConfigsPack = related_configs_pack_t<RelatedKey>;
        
        #define RESOLUTION_ATTEMPT      \
            this->perform_piping(       \
                Pack<RelatedEntity>{},   \
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
    [[nodiscard]] constexpr Reference<RelatedEntity> auto
        perform_piping(
            Pack<RelatedEntity>&&,
            Pack<HeadConfig, TailConfigs...>&&, 
            Reference<RelatedEntity> auto entity
        ) const 
    {
        const HeadConfig& 
            config = static_cast<const HeadConfig&>(*this);
        
        Reference<RelatedEntity> auto 
            processed_entity = config.pipe(entity);

        if constexpr (sizeof...(TailConfigs) == 0)
        {
            return processed_entity;
        } 
        else 
        {
            return this->perform_piping(
                Pack<RelatedEntity>{},
                Pack<TailConfigs...>{}, 
                processed_entity
            );
        }
    }
};

}

#endif // !CHAINABLE_CONFIG_DISPATCHER_HPP_

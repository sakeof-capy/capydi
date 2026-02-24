#ifndef CHAINABLE_CONFIG_DISPATCHER_HPP_
#define CHAINABLE_CONFIG_DISPATCHER_HPP_

#include "capydi/configs/concepts/ChainableConfig.hpp"
#include "capydi/Resolution.hpp"
#include "capydi/Error.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/primitives/referencing/Reference.hpp>
#include <capymeta/primitives/Template.hpp>
#include <capymeta/algorithms/pack/Filter.hpp>
#include <capymeta/concepts/WrappedWIth.hpp>
#include <boost/mp11.hpp>
#include <concepts>
#include <expected>
#include <variant>
#include <tuple>

namespace capy::di
{

template<ChainableConfig... Configs>
class ChainableConfigDispatcher
{
public:
    constexpr explicit ChainableConfigDispatcher( 
        Configs&&... configs
    )
        : configs_tuple_ { std::move(configs)... }
        , configs_dispatch_map_ { 
            populate_configs_map(this->configs_tuple_) 
        }
    {}

public:
    template<typename RelatedKey, typename RelatedEntity>
    [[nodiscard]] constexpr Resolution<
        RelatedEntity, 
        Error
    > auto
        apply_configs_chain(
            meta::Reference<RelatedEntity> auto entity,
            meta::wrapped_with<std::tuple> auto&& input
        ) const 
    {
        auto maybe_configs_array = this->configs_dispatch_map_
            .static_find(meta::Unit<RelatedKey>{});

        if constexpr (!maybe_configs_array.has_value()) [[unlikely]]
        {
            return std::expected<meta::RuntimeRef<RelatedEntity>, Error> {
                entity
            };
        }
        else
        {
            auto configs_array_reference = maybe_configs_array.value();
            typename decltype(configs_array_reference)::ReferenceType configs_array = configs_array_reference;
            return this->perform_piping<RelatedEntity>(configs_array, input, entity, 0);
        }
    }

    template<typename RelatedEntity>
    [[nodiscard]] constexpr Resolution<
        RelatedEntity, 
        Error
    > auto
        perform_piping(
            const auto& configs_array,
            const auto& input,
            meta::Reference<RelatedEntity> auto entity,
            std::size_t current_index
        ) const 
    {
        if (current_index >= configs_array.size())
        {
            return std::expected<meta::RuntimeRef<RelatedEntity>, Error> { 
                entity 
            };
        }

        return 
            std::visit(
                [&entity, &input](const auto& config_reference) {
                    typename std::decay_t<decltype(config_reference)>::ReferenceType config = config_reference;
                    return config.pipe(entity, input);
                },
                configs_array[current_index]
            )
            .and_then([this, &configs_array, current_index, &input](
                meta::Reference<RelatedEntity> auto processed_entity
            ) {
                return this->perform_piping<RelatedEntity>(
                    configs_array, 
                    input,
                    processed_entity, 
                    current_index + 1
                );
            });
    }

private:
    template<typename UniqueType, typename... NonUniqueConfigs>
    static constexpr auto collect(NonUniqueConfigs&... args)
    {
        using namespace boost::mp11;

        auto configs_tuple = std::tuple_cat(
            ([&]() {
                if constexpr (meta::pack_contains_t<
                    get_related_keys_pack_t<NonUniqueConfigs>,
                    UniqueType
                >) {
                    return std::tuple<meta::RuntimeRef<NonUniqueConfigs>> { 
                        meta::RuntimeRef<NonUniqueConfigs> { args } 
                    };
                }
                else {
                    return std::tuple<>{};
                }
            }())...
        );

        return std::apply(
            []<typename... T>(T&&... configs) {
                using UniqueTs = mp_unique<mp_list<std::decay_t<T>...>>;
                using VariantType = meta::rebind_t<
                    UniqueTs, 
                    mp_list, 
                    std::variant
                >;
                return std::array<VariantType, sizeof...(configs)> { 
                    VariantType { std::forward<T>(configs) }...
                };
            },
            std::move(configs_tuple)
        );
    }

    template<meta::wrapped_with<std::tuple> ConfigsTuple>
    static constexpr auto populate_configs_map(
        ConfigsTuple& configs_tuple
    ) {
        using namespace boost::mp11;

        using KeysList = mp_flatten<mp_list<
            meta::rebind_t<
                get_related_keys_pack_t<Configs>, 
                meta::Pack, 
                mp_list
            >...
        >>;

        using UniqueKeysList = mp_unique<KeysList>;

        return [&]<typename... UniqueKeys>(mp_list<UniqueKeys...>&& list) {
            return meta::MetaMap {
                meta::KVPair {
                    meta::Unit<UniqueKeys>{},
                    std::apply(
                        [](auto&... args) {
                            return collect<UniqueKeys>(args...);
                        },
                        configs_tuple
                    )
                }...
            };
        }(UniqueKeysList{});
    }

private:
    using ConfigsTupleType = std::tuple<Configs...>;
    using MapType = decltype(populate_configs_map(std::declval<ConfigsTupleType&>()));

    ConfigsTupleType configs_tuple_;
    MapType configs_dispatch_map_; 
};

}

#endif // !CHAINABLE_CONFIG_DISPATCHER_HPP_

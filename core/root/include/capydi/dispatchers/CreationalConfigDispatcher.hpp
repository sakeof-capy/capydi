#ifndef CREATIONAL_CONFIG_DISPATCHER_HPP_
#define CREATIONAL_CONFIG_DISPATCHER_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/configs/inputs/NoInput.hpp"
#include "capydi/configs/ResolutionOverrides.hpp"
#include "capydi/Resolution.hpp"
#include "capydi/Error.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/type_structures/MetaMap.hpp>
#include <capymeta/type_structures/Maybe.hpp>
#include <capymeta/primitives/referencing/RuntimeRef.hpp>
#include <capymeta/algorithms/pack/ValuedPackFor.hpp>
#include <capymeta/algorithms/pack/legacy/FunctionTraits.hpp>
#include <expected>
#include <utility>
#include <boost/mp11.hpp>
#include <variant>

#include <iostream>

namespace capy::di
{

template<typename T>
concept Creatable = meta::create_static_method_exists_and_is_unique_v<T>;

template<CreationalConfig... Configs>
class CreationalConfigDispatcher
{
public:
    constexpr explicit CreationalConfigDispatcher(
        Configs&&... configs
    )
        : configs_tuple_ { std::move(configs)... }
        , configs_dispatch_map_ { 
            populate_configs_map(this->configs_tuple_) 
        }
    {}

public:
    
    template<
        typename Type, 
        typename KeyPack = meta::Pack<Type>, 
        typename InputType = std::tuple<>
    >
    constexpr Resolution<Type, Error> auto resolve(
        InputType&& input_tuple = std::tuple{}
    ) const {
        auto maybe_config = this->configs_dispatch_map_.static_find(meta::Unit<KeyPack>{});

        if constexpr (!decltype(maybe_config)::has_value())
        {
            return std::expected<meta::RuntimeRef<Type>, Error> {
                std::unexpected { Error::CANNOT_BE_RESOLVED }
            };
        }
        else 
        {
            Error code = Error::CANNOT_BE_RESOLVED;
            auto configs_array_reference = maybe_config.value();
            typename decltype(configs_array_reference)::ReferenceType configs_array = configs_array_reference;

            ResolutionOverrides overrides = std::apply(
                []<typename... T>(T&&... input_args) {
                    return ResolutionOverrides {
                        std::forward<T>(input_args)...
                    };
                },
                std::move(input_tuple)
            );

            for (auto& config_variant : configs_array)
            {
                auto resolution = std::visit([this, &overrides](auto& config_reference) {
                    typename std::decay_t<decltype(config_reference)>::ReferenceType config = config_reference;
                    using DependenciesPack = dependencies_pack_t<std::remove_reference_t<decltype(config)>>;

                    auto maybe_dependencies_tuple = this->resolve_dependencies_tuple(
                        config,
                        DependenciesPack{}
                    );

                    return maybe_dependencies_tuple
                        .and_then([&config, &overrides](auto&& dependencies_tuple) {
                            return config.do_resolve(KeyPack{}, dependencies_tuple, overrides);
                        });
                }, config_variant);


                if (!resolution.has_value()) [[unlikely]]
                {
                    code = resolution.error();
                    continue;
                }
    
                if (!overrides.validate()) [[unlikely]]
                {
                    code = Error::NOT_ALL_INPUTS_RETRIEVED;
                    continue;
                }

                overrides.reset();
                return resolution;
            }

            return std::expected<meta::RuntimeRef<Type>, Error> {
                std::unexpected { code }
            };
        }
    }

private:

    template<typename... Dependencies>
    constexpr meta::wrapped_with<std::expected> auto
        resolve_dependencies_tuple(const auto& config, meta::Pack<Dependencies&...>&&) const
    {
        auto dependencies_tuple = [this, &config]<std::size_t... Idx>(std::index_sequence<Idx...>) {
            return std::tuple {
                [this, &config] {
                    auto dependencies_input = config.template get_dependencies_input<Idx>();

                    if (dependencies_input.has_value()) {
                        return this->resolve<Dependencies>(dependencies_input.value());
                    } else {
                        return this->resolve<Dependencies>();
                    }
                }()...
            };
        }(std::index_sequence_for<Dependencies...>{});

        return std::apply(
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
                        std::unexpected { Error::DEPENDENCY_CANNOT_BE_RESOLVED }
                    };
                }
            },
            std::move(dependencies_tuple)
        );
    }

    template<typename UniqueType, typename... NonUniqueConfigs>
    static constexpr auto collect(NonUniqueConfigs&... args)
    {
        auto configs_tuple = std::tuple_cat(
            ([&]() {
                // static_assert(std::same_as<UniqueType, std::decay_t<NonUniqueConfigs>>);

                if constexpr (meta::pack_contains_t<
                    resolution_keys_pack_t<NonUniqueConfigs>,
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
                using VariantType = std::variant<std::decay_t<T>...>;
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
                resolution_keys_pack_t<Configs>, 
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

#endif // !CREATIONAL_CONFIG_DISPATCHER_HPP_

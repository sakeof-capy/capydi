#ifndef CREATIONAL_CONFIG_DISPATCHER_HPP_
#define CREATIONAL_CONFIG_DISPATCHER_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/configs/inputs/NoInput.hpp"
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

namespace capy::di
{

template<typename T>
concept Creatable = meta::create_static_method_exists_and_is_unique_v<T>;

template<CreationalConfig... Configs>
class CreationalConfigDispatcher
{
private:
    template<typename T>
    using dependencies_of_t = meta::args_pack_t<decltype(T::create)>;

public:
    constexpr explicit CreationalConfigDispatcher(
        Configs&&... configs
    )
        : configs_map_ (
            meta::MultyKVPair {
                resolution_keys_pack_t<Configs>{}, 
                std::forward<Configs>(configs) 
            }...
        )
    {}

public:
    
    template<Creatable Type, typename KeyPack = meta::Pack<Type>, typename InputType = NoInputStub>
    constexpr Resolution<Type, Error> auto resolve(const InputType& input = NoInputStub{}) const
    {
        auto maybe_config = this->configs_map_.static_find(meta::Unit<KeyPack>{});

        if constexpr (!decltype(maybe_config)::has_value())
        {
            return std::expected<meta::RuntimeRef<Type>, Error> {
                std::unexpected { Error::CANNOT_BE_RESOLVED }
            };
        }
        else 
        {
            auto config_reference = maybe_config.value();
            typename decltype(config_reference)::ReferenceType config = config_reference;

            auto maybe_dependencies_tuple = this->resolve_dependencies_tuple(
                config,
                dependencies_of_t<Type>{}
            );

            return maybe_dependencies_tuple
                .and_then([&config, &input](auto&& dependencies_tuple) {
                    return config.do_resolve(KeyPack{}, dependencies_tuple, std::optional { input });
                });
        }
    }

private:

    template<typename... Dependencies>
    constexpr meta::wrapped_with<std::expected> auto
        resolve_dependencies_tuple(const auto& config, meta::Pack<Dependencies&...>&&) const
    {
        auto dependencies_tuple = [this, &config]<std::size_t... Idx>(std::index_sequence<Idx...>) {
            return std::tuple {
                this->resolve<Dependencies>(config.template get_dependencies_input<Idx>())...
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

private:
    meta::MetaMap<
        meta::MultyKVPair<resolution_keys_pack_t<Configs>, Configs>...
    > configs_map_; 
};

}

#endif // !CREATIONAL_CONFIG_DISPATCHER_HPP_

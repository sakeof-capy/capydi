#ifndef CREATIONAL_CONFIG_DISPATCHER_HPP_
#define CREATIONAL_CONFIG_DISPATCHER_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/Resolution.hpp"
#include "capydi/Error.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/type_structures/MetaMap.hpp>
#include <capymeta/primitives/referencing/RuntimeRef.hpp>
#include <capymeta/algorithms/pack/ValuedPackFor.hpp>
#include <capymeta/algorithms/pack/legacy/FunctionTraits.hpp>
#include <expected>

namespace capy::di
{

template<typename T>
concept Creatable = meta::create_static_method_exists_and_is_unique_v<T>;

template<CreationalConfig... Configs>
class CreationalConfigDispatcher : private Configs...
{
private:
    using Configs::do_resolve...;

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
    
    template<Creatable Type, typename KeyPack = meta::Pack<Type>>
    constexpr Resolution<Type, Error> auto resolve() const
    {
        auto maybe_config = this->configs_map_.static_find(meta::Unit<KeyPack>{});
        auto maybe_dependencies_tuple = this->resolve_dependencies_tuple(dependencies_of_t<Type>{});

        if constexpr (!decltype(maybe_config)::has_value())
        {
            return meta::StaticError<meta::RuntimeRef<Type>, Error> {
                Error::CANNOT_BE_RESOLVED
            };
        }
        else if constexpr(!decltype(maybe_dependencies_tuple)::has_value())
        {
            return meta::StaticError<meta::RuntimeRef<Type>, Error> {
                std::move(maybe_dependencies_tuple).error()
            };
        }
        else 
        {
            auto dependencies_tuple = std::move(maybe_dependencies_tuple).value();
            auto config_reference = maybe_config.value();
            typename decltype(config_reference)::ReferenceType config = config_reference;
            return config.do_resolve(KeyPack{}, dependencies_tuple);
        }
    }

private:

    template<typename... Dependencies>
    constexpr meta::wrapped_with<meta::StaticEither> auto
        resolve_dependencies_tuple(meta::Pack<Dependencies&...>&&) const
    {
        return std::apply(
            [](auto&&... maybe_dependencies) {
                using DependenciesTuple = std::tuple<
                    typename std::remove_reference_t<decltype(maybe_dependencies)>::value_type...
                >;

                if constexpr ((std::remove_reference_t<decltype(maybe_dependencies)>::has_value() && ...))
                {
                    return meta::StaticOk<DependenciesTuple, Error> {
                        std::tuple { std::move(maybe_dependencies.value())... }
                    };
                }
                else 
                {
                    return meta::StaticError<DependenciesTuple, Error> {
                        Error::DEPENDENCY_CANNOT_BE_RESOLVED
                    };
                }
            },
            std::tuple {
                this->resolve<Dependencies>()...
            }
        );
    }

private:
    meta::MetaMap<
        meta::MultyKVPair<resolution_keys_pack_t<Configs>, Configs>...
    > configs_map_; 
};

}

#endif // !CREATIONAL_CONFIG_DISPATCHER_HPP_

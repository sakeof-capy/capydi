#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"

#include <capymeta/primitives/Pack.hpp>

namespace capy::di
{

template<CreationalConfig Decoratee, typename Interface>
    requires std::derived_from<central_type_t<Decoratee>, Interface>
class AsInterface
{
public:
    constexpr explicit AsInterface(meta::Unit<Interface>, Decoratee&& decoratee)
        : decoratee_ { std::move(decoratee) }
    {}

public:
    using CentralType = Interface;
    using /* meta::Pack<meta::Pack<?>> */ ResolutionKeysPack = meta::Pack<
        meta::Pack<Interface>, 
        meta::Pack<const Interface>
    >;
    using DependenciesPack = dependencies_pack_t<Decoratee>;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;

public:
    auto do_resolve(
        meta::Pack<Interface>&& keys, 
        auto& dependencies,
        const auto& input
    ) const
    {
        return decoratee_.do_resolve(
            meta::Pack<central_type_t<Decoratee>>{}, 
            dependencies, 
            std::optional<NoInputStub>{}
        )
        .transform([](auto ref) {
            return meta::RuntimeRef<Interface> {
                static_cast<central_type_t<Decoratee>&>(ref)
            };
        });
    }
    
    auto do_resolve(
        meta::Pack<const Interface>&& keys, 
        auto& dependencies,
        const auto& input
    ) const
    {
        return decoratee_.do_resolve(
            meta::Pack<const central_type_t<Decoratee>>{}, 
            dependencies, 
            std::optional<NoInputStub>{}
        )
        .transform([](auto ref) {
            return meta::RuntimeRef<const Interface> {
                static_cast<const central_type_t<Decoratee>&>(ref)
            };
        });
    }

    template<std::size_t DependencyIndex>
    std::optional<NoInputStub> get_dependencies_input() const
    {
        return std::nullopt;    
    }

private:
    Decoratee decoratee_;
};

}

#endif // INTERFACE_HPP_

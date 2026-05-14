#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/configs/decorative/DecoratableConfig.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <tuple>

namespace capy::di
{

namespace implementation_details_
{

template<CreationalConfig Decoratee, typename Interface>
    // requires std::derived_from<central_type_t<Decoratee>, Interface>
class AsInterface
    : public DecoratableConfig<
        AsInterface<Decoratee, Interface>
    >
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
        meta::Pack<Interface> keys, 
        auto& dependencies,
        meta::wrapped_with<ResolutionContext> auto& context,
        const auto& input
    ) const
    {
        return decoratee_.do_resolve(
            meta::Pack<central_type_t<Decoratee>>{}, 
            dependencies, 
            context,
            input
        )
        .transform([](auto ref) {
            return meta::RuntimeRef<Interface> {
                static_cast<central_type_t<Decoratee>&>(ref)
            };
        });
    }
    
    auto do_resolve(
        meta::Pack<const Interface> keys, 
        auto& dependencies,
        meta::wrapped_with<ResolutionContext> auto& context,
        const auto& input
    ) const
    {
        return decoratee_.do_resolve(
            meta::Pack<const central_type_t<Decoratee>>{}, 
            dependencies, 
            context,
            input
        )
        .transform([](auto ref) {
            return meta::RuntimeRef<const Interface> {
                static_cast<const central_type_t<Decoratee>&>(ref)
            };
        });
    }

    template<std::size_t DependencyIndex>
    meta::wrapped_with<std::optional> auto get_dependencies_input() const
    {
        return this->decoratee_
            .template get_dependencies_input<DependencyIndex>();    
    }

private:
    Decoratee decoratee_;
};

}

struct Interface
{
    template<CreationalConfig Decoratee, typename InterfaceType>
    static constexpr implementation_details_::AsInterface<Decoratee, InterfaceType> decorate(
        Decoratee&& decoratee, 
        meta::Unit<InterfaceType> interface_deducer
    ) {
        return implementation_details_::AsInterface<Decoratee, InterfaceType> {
            interface_deducer,
            std::forward<Decoratee>(decoratee)
        };
    }
};

}

#endif // INTERFACE_HPP_

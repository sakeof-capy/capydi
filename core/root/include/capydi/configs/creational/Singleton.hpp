#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/configs/ConfigType.hpp"

#include <capymeta/algorithms/pack/legacy/FunctionTraits.hpp>
#include <capymeta/type_structures/Maybe.hpp>
#include <capymeta/primitives/referencing/RuntimeRef.hpp>
#include <capymeta/primitives/Pack.hpp>
#include <tuple>
#include <expected>

namespace capy::di
{

template<typename Type>
class Singleton 
    : public DecoratableConfig<
        Singleton<Type>
    >
{
public:
    using CentralType = Type;
    using /* meta::Pack<meta::Pack<?>> */ ResolutionKeysPack = meta::Pack<
        meta::Pack<Type>, 
        meta::Pack<const Type>
    >;

    using DependenciesPack = meta::args_pack_t<decltype(CentralType::create)>;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;

public:
    template<typename... Dependencies>
    std::expected<meta::RuntimeRef<Type>, Error> do_resolve(
        meta::Pack<Type> keys, 
        std::tuple<Dependencies...>& dependencies,
        meta::wrapped_with<ResolutionContext> auto& context,
        const auto& input
    ) const
    {
        if (!this->singleton_value_.has_value())
        {
            this->singleton_value_.emplace(
                std::apply(Type::create, dependencies)
            );

            context.flags.just_created = true;
        }

        return meta::RuntimeRef<Type> { this->singleton_value_.value() };
    }

    template<typename... Dependencies>
    std::expected<meta::RuntimeRef<const Type>, Error> do_resolve(
        meta::Pack<const Type> keys, 
        std::tuple<Dependencies...>& dependencies,
        meta::wrapped_with<ResolutionContext> auto& context,
        const auto& input
    ) const
    {
        return meta::RuntimeRef<const Type> {
            this->do_resolve(meta::Pack<Type>{}, dependencies, context, input).value()
        };
    }

    template<std::size_t DependencyIndex>
    std::optional<std::tuple<>> get_dependencies_input() const
    {
        return std::nullopt;    
    }

private:
    mutable std::optional<Type> singleton_value_ = std::nullopt;
};

}

#ifndef GENERATE_UNIQUE_NAME
#   define GENERATE_UNIQUE_NAME SOME_UNIQUE_NAME
#endif

#endif // !SINGLETON_HPP
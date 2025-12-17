#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "capydi/referencing/RuntimeRef.hpp"
#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/configs/ConfigType.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <tuple>

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

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;

public:
    template<typename... Dependencies>
    RuntimeRef<Type> do_resolve(
        meta::Pack<Type> keys, 
        std::tuple<Dependencies...>& dependencies
    ) const
    {
        static Type instance = std::apply(Type::create, dependencies);
        return instance;
    }

    template<typename... Dependencies>
    RuntimeRef<const Type> do_resolve(
        meta::Pack<const Type> keys, 
        std::tuple<Dependencies...>& dependencies
    ) const
    {
        return this->do_resolve(meta::Pack<Type>{}, dependencies);
    }
};

}

#endif // !SINGLETON_HPP
#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/configs/ConfigType.hpp"

#include <capymeta/type_structures/StaticEither.hpp>
#include <capymeta/primitives/referencing/RuntimeRef.hpp>
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
    meta::StaticOk<meta::RuntimeRef<Type>, Error> do_resolve(
        meta::Pack<Type> keys, 
        std::tuple<Dependencies...>& dependencies
    ) const
    {
        static Type instance = std::apply(Type::create, dependencies);
        return meta::RuntimeRef<Type> { instance };
    }

    template<typename... Dependencies>
    meta::StaticOk<meta::RuntimeRef<const Type>, Error> do_resolve(
        meta::Pack<const Type> keys, 
        std::tuple<Dependencies...>& dependencies
    ) const
    {
        return meta::RuntimeRef<const Type> {
            this->do_resolve(meta::Pack<Type>{}, dependencies).value()
        };
    }
};

}

#endif // !SINGLETON_HPP
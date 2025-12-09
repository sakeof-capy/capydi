#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "utilities/pack/Pack.hpp"
#include "utilities/referencing/RuntimeRef.hpp"
#include "configs/decorative/DecoratableConfig.hpp"
#include "configs/ConfigType.hpp"
#include <tuple>

namespace capy::di
{

template<typename Type>
struct Singleton 
    : DecoratableConfig<
        Singleton<Type>
    >
{
public:
    using CentralType = Type;
    using /* Pack<Pack<?>> */ ResolutionKeysPack = Pack<
        Pack<Type>, 
        Pack<const Type>
    >;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;

public:
    template<typename... Dependencies>
    RuntimeRef<Type> do_resolve(
        Pack<Type> keys, 
        std::tuple<Dependencies...>& dependencies
    ) const
    {
        static Type instance = std::apply(Type::create, dependencies);
        return instance;
    }

    template<typename... Dependencies>
    RuntimeRef<const Type> do_resolve(
        Pack<const Type> keys, 
        std::tuple<Dependencies...>& dependencies
    ) const
    {
        return this->do_resolve(Pack<Type>{}, dependencies);
    }
};

}

#endif // !SINGLETON_HPP
#ifndef CONSTLETON_HPP_
#define CONSTLETON_HPP_

#include "utilities/pack/Pack.hpp"
#include "utilities/pack/PackAlgorithm.hpp"
#include "utilities/referencing/ConstexprRef.hpp"
#include "utilities/referencing/Reference.hpp"
#include "configs/decorative/DecoratableConfig.hpp"
#include "configs/ConfigType.hpp"

#include <tuple>
#include <type_traits>

namespace capy::di
{

template<typename T>
struct IsConstexprReference : std::false_type {};

template<typename T, const T& Ref>
struct IsConstexprReference<ConstexprRef<T, Ref>> : std::true_type {};

template<typename T>
constexpr bool is_constexpr_reference_v = IsConstexprReference<std::remove_cvref_t<T>>::value;

// TODO: stop using ConstexprReference. Avoid tight coupling with ConstexprRef.
template<typename T>
concept ConstexprReference = is_constexpr_reference_v<T>;


// template<class Lambda, int=(Lambda{}(), 0)>
// constexpr bool is_constexpr(Lambda) { return true; }
// constexpr bool is_constexpr(...) { return false; }

template<typename Type>
struct Constleton
    : DecoratableConfig<
        Constleton<Type>
    >
{
// TODO: check for well-formness of Constleton (if it is constexpr)

public:
    using CentralType = const Type;
    using /* Pack<Pack<?>> */ ResolutionKeysPack = Pack<
        Pack<CentralType>
    >;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;

public:
    template<ConstexprReference... Dependencies>
    constexpr Reference<CentralType> auto
        do_resolve(
            Pack<CentralType>&& keys, 
            const std::tuple<Dependencies...>& dependencies
        ) const
    {
        static constexpr CentralType instance = std::apply(Type::create, dependencies);
        return ConstexprRef<CentralType, instance>{};
    }
};

}

#endif // !CONSTLETON_HPP_
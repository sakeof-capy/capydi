#ifndef CONSTLETON_HPP_
#define CONSTLETON_HPP_

#include <capymeta/primitives/referencing/ConstexprRef.hpp>
#include <capymeta/primitives/referencing/RuntimeRef.hpp>
#include <capymeta/primitives/referencing/Reference.hpp>
#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/configs/ConfigType.hpp"

#include <capymeta/type_structures/StaticEither.hpp>
#include <capymeta/primitives/Pack.hpp>
#include <tuple>
#include <type_traits>

namespace capy::di
{

template<typename T>
struct IsConstexprReference : std::false_type {};

template<typename T, const T& Ref>
struct IsConstexprReference<meta::ConstexprRef<T, Ref>> : std::true_type {};

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
    : public DecoratableConfig<
        Constleton<Type>
    >
{
// TODO: check for well-formness of Constleton (if it is constexpr)

public:
    using CentralType = const Type;
    using /* meta::Pack<meta::Pack<?>> */ ResolutionKeysPack = meta::Pack<
        meta::Pack<CentralType>
    >;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;

public:
    template<ConstexprReference... Dependencies>
    constexpr meta::wrapped_with<meta::StaticEither> auto
        do_resolve(
            meta::Pack<CentralType>&& keys, 
            const std::tuple<Dependencies...>& dependencies
        ) const
    {
        static constexpr CentralType instance = std::apply(Type::create, dependencies);
        return meta::StaticOk<meta::ConstexprRef<CentralType, instance>, Error> {
            meta::ConstexprRef<CentralType, instance>{}
        };
    }

    // TODO: think of simplifying this one
    //       can we replace tuple with just auto?
    template<typename... Dependencies>
    constexpr meta::wrapped_with<meta::StaticEither> auto
        do_resolve(
            meta::Pack<CentralType>&& keys, 
            const std::tuple<Dependencies...>& dependencies
        ) const
    {
        return meta::StaticError<meta::RuntimeRef<CentralType>, Error> {
            Error::CONSTLETON_ERROR
        };
    }
};

}

#endif // !CONSTLETON_HPP_
#ifndef CONSTLETON_HPP_
#define CONSTLETON_HPP_

#include "utilities/pack/Pack.hpp"
#include "utilities/pack/PackAlgorithm.hpp"
#include "utilities/referencing/ConstexprRef.hpp"
#include "utilities/referencing/Reference.hpp"

#include <tuple>

namespace capy::di
{

// template<class Lambda, int=(Lambda{}(), 0)>
// constexpr bool is_constexpr(Lambda) { return true; }
// constexpr bool is_constexpr(...) { return false; }

template<typename Type>
struct Constleton
{
// TODO: check for well-formness of Constleton (if it is constexpr)

protected:
    template<typename... Dependencies>
    constexpr Reference<Type> auto
        do_resolve(
            Pack<const Type> keys, 
            const std::tuple<Dependencies...>& dependencies
        ) const
    {
        static constexpr Type instance = std::apply(Type::create, dependencies);
        return ConstexprRef<Type, instance>{};
    }
};

}

#endif // !SINGLETON_HPP
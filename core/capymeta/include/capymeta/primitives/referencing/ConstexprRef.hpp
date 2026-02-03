#ifndef CONSTEXPR_REF_HPP_
#define CONSTEXPR_REF_HPP_

#include "Reference.hpp"

#include <type_traits>

namespace capy::meta
{

template<typename T, const T& Ref>
    requires std::is_const_v<T>
struct ConstexprRef
{
public:
    using ReferentType = T;
    using ReferenceType = const T&;
    using ConstReferenceType = const T&;

public:
    constexpr operator ConstReferenceType() const noexcept
    {
        return Ref;
    }
};

}

#endif // !CONSTEXPR_REF_HPP_

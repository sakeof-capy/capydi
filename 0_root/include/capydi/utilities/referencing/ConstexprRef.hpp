#ifndef CONSTEXPR_REF_HPP_
#define CONSTEXPR_REF_HPP_

#include "Reference.hpp"

namespace capy::di
{

template<typename T, const T& Ref>
struct ConstexprRef
{
public:
    using ReferentType = T;
    using ReferenceType = T&;
    using ConstReferenceType = const T&;

public:
    constexpr operator ConstReferenceType() const noexcept
    {
        return Ref;
    }
};

}

#endif // !CONSTEXPR_REF_HPP_

#ifndef RUNTIME_REF_HPP_
#define RUNTIME_REF_HPP_

#include "Reference.hpp"

#include <type_traits>

namespace capy::meta
{

template<typename T>
class RuntimeRef 
{
public:
    using ReferentType = T;
    using ReferenceType = T&;
    using ConstReferenceType = const T&;
    
private:
    static constexpr bool IS_CONST = std::is_const_v<ReferentType>;

public:
    constexpr /* implicit */ RuntimeRef(ReferenceType ref)
        : ref_ { ref }
    {}

public:
    constexpr 
        operator ReferenceType() const noexcept requires (!IS_CONST) 
    {
        return ref_;
    }

    constexpr 
        operator ConstReferenceType() const noexcept
    {
        return ref_;
    }

    constexpr 
        operator RuntimeRef<const T>() const noexcept
    {
        return RuntimeRef<const T> { ref_ };
    }

private:
    ReferenceType ref_;
};

}

#endif // !RUNTIME_REF_HPP_

/**
 * @file RuntimeRef.hpp
 * @brief Runtime reference wrapper for dependency instances.
 * 
 * RuntimeRef is used for normal object resolution at runtime. It stores a reference
 * to an object and satisfies the Reference concept, making it suitable for all
 * standard dependency injection scenarios.
 */

#ifndef RUNTIME_REF_HPP_
#define RUNTIME_REF_HPP_

#include "Reference.hpp"

#include <type_traits>

namespace capy::di
{

/**
 * @class RuntimeRef
 * @brief A runtime reference wrapper satisfying the Reference concept.
 * 
 * @tparam T The type being referenced.
 * 
 * RuntimeRef is the standard reference type used in typical DI scenarios. It wraps
 * an lvalue reference and provides implicit conversions to mutable and const references.
 * This enables seamless use in decorator chains and resolution pipelines.
 * 
 * @code
 * RuntimeRef<MyService> ref { my_service_instance };
 * MyService& s = ref;                    // Implicit conversion to T&
 * const MyService& cs = ref;             // Implicit conversion to const T&
 * RuntimeRef<const MyService> cref = ref; // Conversion to const version
 * @endcode
 * 
 * @tparam T The referent type. If @p T is const, the reference is const-qualified.
 * 
 * @note This type is @c constexpr-capable, allowing it to be used in compile-time
 *       contexts if the underlying reference permits.
 * 
 * @see Reference
 * @see ConstexprRef
 */
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
    /// @brief Implicit constructor from an lvalue reference
    constexpr /* implicit */ RuntimeRef(ReferenceType ref)
        : ref_ { ref }
    {}

public:
    /// @brief Conversion to mutable reference (enabled only if T is not const)
    constexpr 
        operator ReferenceType() const noexcept requires (!IS_CONST) 
    {
        return ref_;
    }

    /// @brief Conversion to const reference
    constexpr 
        operator ConstReferenceType() const noexcept
    {
        return ref_;
    }

    /// @brief Conversion to a const RuntimeRef wrapper
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

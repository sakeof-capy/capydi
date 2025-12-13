/**
 * @file ConstexprRef.hpp
 * @brief Compile-time constant reference wrapper for static dependency instances.
 * 
 * ConstexprRef enables compile-time dependency resolution by holding a reference
 * to a compile-time constant object. This is used when dependencies are known and
 * can be statically initialized.
 */

#ifndef CONSTEXPR_REF_HPP_
#define CONSTEXPR_REF_HPP_

#include "Reference.hpp"

#include <type_traits>

namespace capy::di
{

/**
 * @class ConstexprRef
 * @brief A compile-time reference to a static, compile-time constant object.
 * 
 * @tparam T The type of the referenced object.
 * @tparam Ref A compile-time constant reference to an object of type T.
 * 
 * ConstexprRef is used for dependency injection scenarios where dependencies are
 * known at compile time and can be stored as static/global constants. This is
 * particularly useful for:
 * - Configuration objects that never change
 * - Singleton instances with static storage
 * - Constants and compile-time data structures
 * 
 * @note Unlike RuntimeRef, ConstexprRef stores no data; it only wraps a template
 *       parameter that is a reference to a compile-time constant.
 * 
 * @example
 * @code
 * constexpr MyService my_service{};
 * using MyRef = ConstexprRef<const MyService, my_service>;
 * 
 * constexpr const MyService& svc = MyRef{};  // Returns reference to my_service
 * @endcode
 * 
 * @see RuntimeRef
 * @see Reference
 */
template<typename T, const T& Ref>
    requires std::is_const_v<T>
struct ConstexprRef
{
public:
    using ReferentType = T;
    using ReferenceType = const T&;
    using ConstReferenceType = const T&;

public:
    /// @brief Implicit conversion to const reference (the only safe conversion for compile-time constants)
    constexpr operator ConstReferenceType() const noexcept
    {
        return Ref;
    }
};

}

#endif // !CONSTEXPR_REF_HPP_

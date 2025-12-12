/**
 * @file Reference.hpp
 * @brief Core abstraction for type-safe references in the dependency injection system.
 * 
 * The Reference concept provides a unified interface for both compile-time (@c ConstexprRef)
 * and runtime (@c RuntimeRef) references to objects. This enables the DI framework to work
 * uniformly with different reference semantics while maintaining type safety.
 * 
 * All references must declare their referent type, and be convertible to both mutable
 * and const references for maximum flexibility in decorator chains.
 */

#ifndef REFERENCE_HPP_
#define REFERENCE_HPP_

#include <concepts>

namespace capy::di
{

/**
 * @concept Reference
 * @brief Concept for type-safe references that can represent both runtime and compile-time entities.
 * 
 * @tparam Reference_ The reference type being tested.
 * @tparam Referent The type being referred to.
 * 
 * A type satisfies the Reference concept if it:
 * - Provides type aliases: @c ReferentType, @c ReferenceType, @c ConstReferenceType
 * - Correctly identifies the referent type: @c ReferentType == Referent
 * - Provides proper reference aliases: @c ReferenceType == Referent&, @c ConstReferenceType == const Referent&
 * - Is convertible to both mutable and const references of the referent
 * 
 * This enables a powerful abstraction:
 * 
 * @code
 * // The framework can work with either:
 * RuntimeRef<MyService> rt_ref;      // Runtime reference (typical case)
 * ConstexprRef<MyService> ct_ref;    // Compile-time reference (for static DI)
 * 
 * // Both satisfy Reference<?, MyService> and can be used interchangeably
 * MyService& svc = rt_ref;            // Implicit conversion to mutable reference
 * const MyService& csvc = ct_ref;     // Implicit conversion to const reference
 * @endcode
 * 
 * @see RuntimeRef
 * @see ConstexprRef
 * @see Resolution
 */
template<typename Reference_, typename Referent>
concept Reference = 
    requires() {
        typename Reference_::ReferentType;
        typename Reference_::ReferenceType;
        typename Reference_::ConstReferenceType;
    }
    && std::same_as<typename Reference_::ReferentType, Referent>
    && std::same_as<typename Reference_::ReferenceType, Referent&>
    && std::same_as<typename Reference_::ConstReferenceType, const Referent&>
    && std::convertible_to<Reference_, typename Reference_::ReferenceType>
    && std::convertible_to<Reference_, typename Reference_::ConstReferenceType>;

}

#endif // !REFERENCE_HPP_

#ifndef REFERENCE_HPP_
#define REFERENCE_HPP_

#include <concepts>

namespace capy::meta
{

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

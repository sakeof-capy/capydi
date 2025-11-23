#ifndef REFERENCE_HPP_
#define REFERENCE_HPP_

#include <concepts>

namespace capy::di
{

template<typename Reference_, typename Referent>
concept Reference = std::convertible_to<Reference_, const Referent&>;

}

#endif // !REFERENCE_HPP_

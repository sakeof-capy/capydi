#ifndef CAPYDI_CONTAINS_HPP_
#define CAPYDI_CONTAINS_HPP_

#include "Filter.hpp"
#include "capymeta/primitives/Functor.hpp"

#include <concepts>

namespace capy::meta
{

template<typename Pack_, typename Element>
constexpr bool pack_contains_t = !pack_is_empty_v<pack_filter_t<
    Pack_,
    functor_fv<[]<class T>(Pack<T>) {
        return std::same_as<T, Element>;
    }, MetaArity::N1>
>>;

}

#endif // !CAPYDI_CONTAINS_HPP_

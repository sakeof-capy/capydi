#ifndef CAPYDI_CONTAINS_HPP_
#define CAPYDI_CONTAINS_HPP_

#include "Filter.hpp"
#include "capymeta/concepts/Trait.hpp"
#include "capymeta/primitives/Functor.hpp"

namespace capy::meta
{

template<
    typename Pack_,
    v_trait<MetaArity::N1> Predicate
>
constexpr bool pack_contains_like_v = std::same_as<
    pack_filter_t<Pack_, Predicate>,
    Pack<>
>;

template<typename Pack_, typename Element>
constexpr bool pack_contains_v = pack_contains_like_v<
    Pack_,
    functor_fv<[]<class T>(Pack<T>) {
        return std::same_as<T, Element>;
    }, MetaArity::N1>
>;

}

#endif //CAPYDI_CONTAINS_HPP_

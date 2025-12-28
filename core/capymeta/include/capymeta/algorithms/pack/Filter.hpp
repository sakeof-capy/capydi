#ifndef CAPYDI_FILTER_HPP_
#define CAPYDI_FILTER_HPP_

#include "legacy/Filter.hpp"
#include "capymeta/concepts/Trait.hpp"

namespace capy::meta
{

template<
    typename Pack,
    v_trait<MetaArity::N1> Predicate
>
using pack_filter_t = legacy::filter_t<
    Pack,
    as_unary_fv<Predicate>::template Functor
>;

}

#endif //CAPYDI_FILTER_HPP_

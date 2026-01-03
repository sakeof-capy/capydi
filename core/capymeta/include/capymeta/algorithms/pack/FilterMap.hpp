#ifndef CAPYDI_FILTERMAP_HPP_
#define CAPYDI_FILTERMAP_HPP_

#include "legacy/FilterMap.hpp"
#include "capymeta/concepts/Trait.hpp"

namespace capy::meta
{

template<
    typename Pack,
    t_trait<MetaArity::N1> Mapper
>
using pack_filter_map_t = legacy::filter_map_t<
    Pack,
    as_unary_ft<Mapper>::template Functor
>;

}

#endif // !CAPYDI_FILTERMAP_HPP_
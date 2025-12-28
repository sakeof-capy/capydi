#ifndef CAPYDI_MAP_HPP_
#define CAPYDI_MAP_HPP_

#include "legacy/PackMap.hpp"
#include "capymeta/concepts/Trait.hpp"

namespace capy::meta
{

template<
    typename Pack_,
    t_trait<MetaArity::N1> Mapper
>
using pack_map_t = legacy::pack_map_t<
    Pack_,
    as_unary_ft<Mapper>::template Functor
>;

}

#endif //CAPYDI_MAP_HPP_

#ifndef CAPYDI_FIND_HPP_
#define CAPYDI_FIND_HPP_

#include "Filter.hpp"
#include "Head.hpp"
#include "capymeta/concepts/Trait.hpp"

namespace capy::meta
{

template<
    typename Pack_,
    v_trait<MetaArity::N1> Predicate
>
using /* Maybe<?> */ pack_find_t = pack_head_t<
    pack_filter_t<Pack_, Predicate>
>;

}

#endif //CAPYDI_FIND_HPP_

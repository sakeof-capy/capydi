#ifndef CAPYDI_MAP_HPP_
#define CAPYDI_MAP_HPP_

#include "capymeta/concepts/Trait.hpp"
#include "capymeta/common/BoostMP11Adapter.hpp"

#ifndef BOOST_FOUND
#include "legacy/PackMap.hpp"
#endif // !BOOST_FOUND

namespace capy::meta
{

template<
    typename Pack_,
    t_trait<MetaArity::N1> Mapper
>
using pack_map_t
#ifdef BOOST_FOUND
    = boost_adapters::as_pack<boost::mp11::mp_transform<
        boost_adapters::adapt_t_trait_t<MetaArity::N1, Mapper>::template Functor,
        boost_adapters::as_mp_list<Pack_>
    >>;
#else
    = legacy::pack_map_t<
        Pack_,
        as_unary_ft<Mapper>::template Functor
    >;
#endif // !BOOST_FOUND

}

#endif // !CAPYDI_MAP_HPP_

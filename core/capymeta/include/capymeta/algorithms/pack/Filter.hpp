#ifndef CAPYDI_FILTER_HPP_
#define CAPYDI_FILTER_HPP_

#include "capymeta/concepts/Trait.hpp"
#include "capymeta/common/BoostMP11Adapter.hpp"

#ifndef BOOST_FOUND
#include "legacy/Filter.hpp"
#endif // !BOOST_FOUND

namespace capy::meta
{

template<
    typename Pack_,
    v_trait<MetaArity::N1> Predicate
>
using pack_filter_t
#ifdef BOOST_FOUND
    = boost_adapters::as_pack<boost::mp11::mp_filter<
        Predicate::template Functor,
        boost_adapters::as_mp_list<Pack_>
    >>;
#else
    = legacy::filter_t<
        Pack_,
        as_unary_fv<Predicate>::template Functor
    >;
#endif // !BOOST_FOUND

}

#endif // !CAPYDI_FILTER_HPP_

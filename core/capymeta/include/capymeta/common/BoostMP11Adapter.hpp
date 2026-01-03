#ifndef CAPYDI_BOOST_MP11_ADAPTER_HPP_
#define CAPYDI_BOOST_MP11_ADAPTER_HPP_

#ifdef BOOST_FOUND
#include "capymeta/algorithms/general/Rebind.hpp"
#include "capymeta/primitives/Pack.hpp"
#include "capymeta/concepts/Trait.hpp"

#include <boost/mp11.hpp>

namespace capy::meta::boost_adapters
{

template<typename Pack_>
using as_mp_list = rebind_t<Pack_, Pack, boost::mp11::mp_list>;

template<typename MpList>
using as_pack = rebind_t<MpList, boost::mp11::mp_list, Pack>;

template<
    MetaArity ARITY,
    t_trait<ARITY> TypeFunctor
>
struct AdaptTTrait
{
    template<typename... Types>
    using Functor = typename TypeFunctor::template Functor<Types...>::type;
};

template<
    MetaArity ARITY,
    t_trait<ARITY> TypeFunctor
>
using adapt_t_trait_t = AdaptTTrait<ARITY, TypeFunctor>;

}

#endif // BOOST_FOUND

#endif // !CAPYDI_BOOST_MP11_ADAPTER_HPP_

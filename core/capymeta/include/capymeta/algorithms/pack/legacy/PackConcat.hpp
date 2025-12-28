#ifndef PACK_CONCAT_HPP_
#define PACK_CONCAT_HPP_

#include "capymeta/primitives/Pack.hpp"

namespace capy::meta::legacy
{

template<typename Pack1, typename Pack2>
struct PackConcat;

template<typename... Pack1Elements, typename... Pack2Elements>
struct PackConcat<
    Pack<Pack1Elements...>, 
    Pack<Pack2Elements...>
>
{
    using type = Pack<Pack1Elements..., Pack2Elements...>;
};

template<typename Pack1, typename Pack2>
using pack_concat_t = typename PackConcat<Pack1, Pack2>::type;

template<typename... Packs>
struct PacksMultyConcat;

template<typename HeadPack, typename... TailPacks>
struct PacksMultyConcat<HeadPack, TailPacks...>
{
    using type = pack_concat_t<
        HeadPack, 
        typename PacksMultyConcat<TailPacks...>::type
    >;
};

template<>
struct PacksMultyConcat<>
{
    using type = Pack<>;
};

template<typename... Packs>
using packs_multy_concat_t = typename PacksMultyConcat<Packs...>::type;

}

#endif // !PACK_CONCAT_HPP_

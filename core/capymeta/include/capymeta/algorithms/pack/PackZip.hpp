#ifndef PACK_ZIP_HPP_
#define PACK_ZIP_HPP_

#include "capymeta/primitives/Pack.hpp"

namespace capy::meta
{

template<typename Pack1, typename Pack2>
struct PackZip;

template<
    typename... Pack1Elements, 
    typename... Pack2Elements
>
struct PackZip<
    Pack<Pack1Elements...>,
    Pack<Pack2Elements...>
>
{
    using type = Pack<Pack<Pack1Elements, Pack2Elements>...>;
};

template<typename Pack1, typename Pack2>
using pack_zip_t = typename PackZip<Pack1, Pack2>::type; 

}

#endif // !PACK_ZIP_HPP_

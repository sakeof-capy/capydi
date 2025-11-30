#ifndef PACK_CONCAT_HPP_
#define PACK_CONCAT_HPP_

#include "Pack.hpp"

namespace capy::di
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

}

#endif // !PACK_CONCAT_HPP_

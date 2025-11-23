#ifndef PACK_HPP_
#define PACK_HPP_

#include <tuple>

namespace capy::di
{

template<typename UnitType>
struct Unit {};

template<typename... Types>
struct Pack {};

using EmptyPack = Pack<>;

template<typename Pack_, template<typename...> typename Template>
struct RebindPack;

template<template<typename...> typename Template, typename... Elements>
struct RebindPack<Pack<Elements...>, Template>
{
    using type = Template<Elements...>;
};

template<typename Pack_, template<typename...> typename Template>
using rebind_pack_t = typename RebindPack<Pack_, Template>::type;

}

#endif // !PACK_HPP_

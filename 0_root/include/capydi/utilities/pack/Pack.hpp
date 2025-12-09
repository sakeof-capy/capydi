#ifndef PACK_HPP_
#define PACK_HPP_

#include <tuple>

namespace capy::di
{

template<typename UnitType>
struct Unit {};

template<typename... Types>
struct Pack {};

template<auto Value>
struct ValueUnit {};

template<typename ValueUnit_>
struct UnwrapValueUnit;

template<auto Value>
struct UnwrapValueUnit<ValueUnit<Value>>
{
    static constexpr auto value = Value;
};

template<typename ValueUnit_>
static constexpr auto unwrap_value_unit_v = UnwrapValueUnit<ValueUnit_>::value;

template<
    template<typename> typename
> 
struct UnaryMetaFunction {};

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

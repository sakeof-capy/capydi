#ifndef HEAD_HPP_
#define HEAD_HPP_

#include "capymeta/primitives/Pack.hpp"
#include "capymeta/type_structures/Maybe.hpp"

namespace capy::meta
{

template<typename Pack>
struct Head;

template<typename Head_, typename... Tail>
struct Head<Pack<Head_, Tail...>>
{
    using type = Head_;
};

template<>
struct Head<Pack<>>
{
    using type = None;
};

template<typename Pack>
using head_t = typename Head<Pack>::type;

}

#endif // !HEAD_HPP_

#ifndef HEAD_HPP_
#define HEAD_HPP_

#include "Pack.hpp"
#include "None.hpp"

namespace capy::di
{

template<typename Pack>
struct Head;

template<typename Head_, typename... Tail>
struct Head<Pack<Head_, Tail...>>
{
    using type = Head;
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

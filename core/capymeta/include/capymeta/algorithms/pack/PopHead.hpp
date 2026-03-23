#ifndef CAPYDI_POP_HEAD_HPP_
#define CAPYDI_POP_HEAD_HPP_

#include "capymeta/primitives/Pack.hpp"

namespace capy::meta
{

template<typename Pack_>
struct PackPopHead
{
    using type = Pack_;
};

template<typename Head, typename... Tail>
struct PackPopHead<
    Pack<Head, Tail...>
>
{
    using type = Pack<Tail...>;
};

template<typename Pack_>
using pack_pop_head_t = typename PackPopHead<Pack_>::type;

}

#endif // !CAPYDI_POP_HEAD_HPP_

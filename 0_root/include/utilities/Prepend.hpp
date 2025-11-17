#ifndef PREPEND_HPP_
#define PREPEND_HPP_

#include "Pack.hpp"

namespace capy::di
{

template<typename Element, typename Pack>
struct Prepend;

template<typename Element, typename... PackElements>
struct Prepend<Element, Pack<PackElements...>> 
{
    using type = Pack<Element, PackElements...>;
};

template<typename Element, typename Pack>
using prepend_t = typename Prepend<Element, Pack>::type;

}

#endif // !PREPEND_HPP_

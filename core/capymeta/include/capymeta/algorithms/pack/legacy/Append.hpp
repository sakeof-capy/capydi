#ifndef APPEND_HPP_
#define APPEND_HPP_

#include "capymeta/primitives/Pack.hpp"

namespace capy::meta::legacy
{

template<typename Element, typename Pack>
struct Append;

template<typename Element, typename... PackElements>
struct Append<Element, Pack<PackElements...>> 
{
    using type = Pack<PackElements..., Element>;
};

template<typename Element, typename Pack>
using append_t = typename Append<Element, Pack>::type;

}

#endif // !APPEND_HPP_

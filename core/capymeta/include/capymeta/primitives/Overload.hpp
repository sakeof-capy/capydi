#ifndef OVERLOAD_HPP_
#define OVERLOAD_HPP_

#include "capymeta/primitives/Pack.hpp"

namespace capy::meta
{

template<typename... Units>
struct Overload : Units...
{
    using Units::operator()...;
};

}

#endif // !OVERLOAD_HPP_

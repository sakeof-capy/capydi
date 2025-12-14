#ifndef OVERLOAD_HPP_
#define OVERLOAD_HPP_

#include "pack/Pack.hpp"

namespace capy::di
{

template<typename... Units>
struct Overload : Units...
{
    using Units::operator()...;
};

}

#endif // !OVERLOAD_HPP_

#ifndef NONE_HPP_
#define NONE_HPP_

#include <concepts>

namespace capy::di
{

/*
    Type used for marking type absense 
    in case of optional types 
*/ 
struct None {};


template<typename T>
constexpr bool is_none_v = std::same_as<T, None>;

}

#endif // NONE_HPP_

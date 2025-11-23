#ifndef FUNCTION_TYPE_TRAITS_HPP_
#define FUNCTION_TYPE_TRAITS_HPP_

#include "Pack.hpp"

namespace capy::di
{

template<typename T>
struct function_traits;

template<typename ReturnValue_, typename... Params_>
struct function_traits<ReturnValue_(Params_...)>
{
    using ReturnValue = ReturnValue_;
    using Params = Pack<Params_...>;
};

template<typename Function>
using ReturnValueOf = typename function_traits<Function>::ReturnValue;

template<typename Function>
using ParamPackOf = typename function_traits<Function>::Params;

template<typename T>
auto create_static_method_exists_and_is_unique(int*) -> ParamPackOf<decltype(T::create)>;

template<typename T>
auto create_static_method_exists_and_is_unique(...) -> int;

template<typename T>
constexpr bool create_static_method_exists_and_is_unique_v = !std::same_as<
    decltype(create_static_method_exists_and_is_unique<T>(nullptr)),
    int
>;

}

#endif //FUNCTION_TYPE_TRAITS_HPP_

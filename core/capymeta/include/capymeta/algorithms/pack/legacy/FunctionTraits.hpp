#ifndef FUNCTION_TYPE_TRAITS_HPP_
#define FUNCTION_TYPE_TRAITS_HPP_

#include "capymeta/primitives/Pack.hpp"

namespace capy::meta
{

template<typename T>
struct function_traits;

template<typename ReturnValue_, bool IsNoexcept, typename... Params_>
struct function_traits<ReturnValue_(Params_...) noexcept(IsNoexcept)>
{
    using ReturnValue = ReturnValue_;
    using Params = Pack<Params_...>;

    static constexpr bool IS_NOEXCEPT = IsNoexcept;
};

template<typename Function>
using return_value_t = typename function_traits<Function>::ReturnValue;

template<typename Function>
using args_pack_t = typename function_traits<Function>::Params;

template<typename Function>
constexpr bool is_noexcept_v = function_traits<Function>::IS_NOEXCEPT;

template<typename T>
auto create_static_method_exists_and_is_unique(int*) -> args_pack_t<decltype(T::create)>;

template<typename T>
auto create_static_method_exists_and_is_unique(...) -> int;

template<typename T>
constexpr bool create_static_method_exists_and_is_unique_v = !std::same_as<
    decltype(create_static_method_exists_and_is_unique<T>(nullptr)),
    int
>;

}

#endif //FUNCTION_TYPE_TRAITS_HPP_

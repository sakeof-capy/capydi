#ifndef CAPYMETA_WRAPPED_WITH_HPP_
#define CAPYMETA_WRAPPED_WITH_HPP_

#include <type_traits>

namespace capy::meta
{

namespace implementation_details_
{
    template<
        typename Type, 
        template<typename...> typename Wrapper
    >
    struct IsWrappedWith : std::false_type {};

    template<
        template<typename...> typename Wrapper, 
        typename... Wrapees
    >
    struct IsWrappedWith<
        Wrapper<Wrapees...>, 
        Wrapper
    > 
        : std::true_type 
    {};
}

template<typename T, template<typename...> typename Wrapper>
concept wrapped_with = implementation_details_::IsWrappedWith<T, Wrapper>::value;

}

#endif // CAPYMETA_WRAPPED_WITH_HPP_

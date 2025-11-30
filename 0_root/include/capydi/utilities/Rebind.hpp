#ifndef REBIND_HPP_
#define REBIND_HPP_

namespace capy::di
{

template<
    typename Type, 
    template<typename...> typename SrcContainer,
    template<typename...> typename DstContainer
>
struct Rebind;

template<
    template<typename...> typename SrcContainer,
    template<typename...> typename DstContainer,
    typename... Types
>
struct Rebind<
    SrcContainer<Types...>,
    SrcContainer,
    DstContainer
>
{
    using type = DstContainer<Types...>;
};

template<
    typename Type, 
    template<typename...> typename SrcContainer,
    template<typename...> typename DstContainer
>
using rebind_t = typename Rebind<Type, SrcContainer, DstContainer>::type;

}

#endif // !REBIND_HPP_

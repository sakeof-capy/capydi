#ifndef REBIND_HPP_
#define REBIND_HPP_

#include "capydi/utilities/pack/Pack.hpp"

namespace capy::di
{

namespace typed__
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

}

namespace valued__
{

template<
    typename Type, 
    template<auto...> typename SrcContainer,
    template<auto...> typename DstContainer
>
struct Rebind;

template<
    template<auto...> typename SrcContainer,
    template<auto...> typename DstContainer,
    auto... Values
>
struct Rebind<
    SrcContainer<Values...>,
    SrcContainer,
    DstContainer
>
{
    using type = DstContainer<Values...>;
};

}

namespace typed_as_valued__
{

template<
    typename Type, 
    template<typename...> typename SrcContainer,
    template<auto...> typename DstContainer
>
struct Rebind;

template<
    template<typename...> typename SrcContainer,
    template<auto...> typename DstContainer,
    typename... Types
>
struct Rebind<
    SrcContainer<Types...>,
    SrcContainer,
    DstContainer
>
{
    using type = DstContainer<
        unwrap_value_unit_v<Types>...
    >;
};

}


namespace valued_as_typed__
{

template<
    typename Type, 
    template<auto...> typename SrcContainer,
    template<typename...> typename DstContainer
>
struct Rebind;

template<
    template<auto...> typename SrcContainer,
    template<typename...> typename DstContainer,
    auto... Values
>
struct Rebind<
    SrcContainer<Values...>,
    SrcContainer,
    DstContainer
>
{
    using type = DstContainer<ValueUnit<Values>...>;
};

}

template<
    typename Type, 
    template<typename...> typename SrcContainer,
    template<typename...> typename DstContainer
>
using rebind_t = typename typed__::Rebind<
    Type, 
    SrcContainer, 
    DstContainer
>::type;

template<
    typename Type, 
    template<auto...> typename SrcContainer,
    template<auto...> typename DstContainer
>
using valued_rebind_t = typename valued__::Rebind<
    Type, 
    SrcContainer, 
    DstContainer
>::type;

template<
    typename Type, 
    template<typename...> typename SrcContainer,
    template<auto...> typename DstContainer
>
using rebind_typed_as_valued_t = typename typed_as_valued__::Rebind<
    Type, 
    SrcContainer, 
    DstContainer
>::type;

template<
    typename Type, 
    template<auto...> typename SrcContainer,
    template<typename...> typename DstContainer
>
using rebind_valued_as_typed_t = typename valued_as_typed__::Rebind<
    Type, 
    SrcContainer, 
    DstContainer
>::type;

}

#endif // !REBIND_HPP_

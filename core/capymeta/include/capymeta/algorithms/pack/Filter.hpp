#ifndef FILTER_HPP_
#define FILTER_HPP_

#include "capymeta/primitives/Pack.hpp"
#include "Prepend.hpp"

#include <type_traits>

namespace capy::meta
{

template<
    typename Pack, 
    template<typename> typename Predicate
>
struct Filter;

template<
    template<typename> typename Predicate, 
    typename Head,
    typename... Rest
>
struct Filter<Pack<Head, Rest...>, Predicate>
{
    using type = std::conditional_t<
        Predicate<Head>::value,
        prepend_t<Head, typename Filter<Pack<Rest...>, Predicate>::type>,
        typename Filter<Pack<Rest...>, Predicate>::type
    >;
};

template<
    template<typename> typename Predicate
>
struct Filter<Pack<>, Predicate>
{
    using type = Pack<>;
};


template<
    typename Pack, 
    template<typename> typename Predicate
>
using filter_t = typename Filter<Pack, Predicate>::type;

}

#endif // !FILTER_HPP_

#ifndef PACK_CONTAINS_HPP_
#define PACK_CONTAINS_HPP_

#include "Pack.hpp"
#include "Filter.hpp"

namespace capy::di
{

template<typename Element>
struct Equals
{
    template<typename PackElement>
    struct Predicate
    {
        static constexpr bool value = std::same_as<Element, PackElement>;
    };
};

template<
    typename Pack_,
    template<typename> typename Predicate
>
constexpr bool pack_contains_like_t = std::same_as<
    filter_t<Pack_, Predicate>,
    Pack<>
>;

template<typename Pack_, typename Element>
constexpr bool pack_contains_t = pack_contains_like_t<
    Pack_, 
    Equals<Element>::template Predicate
>;

}

#endif // !PACK_CONTAINS_HPP_

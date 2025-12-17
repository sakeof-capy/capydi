#ifndef FILTER_FILTER_MAP_HPP_
#define FILTER_FILTER_MAP_HPP_

#include "capymeta/primitives/Pack.hpp"
#include "capymeta/type_structures/Maybe.hpp"
#include "Prepend.hpp"

namespace capy::meta
{

template<
    typename Pack,
    template<typename> typename Mapper
>
struct FilterMap;

template<
    template<typename> typename Mapper,
    typename Head,
    typename... Tail
>
struct FilterMap<Pack<Head, Tail...>, Mapper>
{
private:
    using /* T */ MappedType = typename Mapper<Head>::type;
    using /* Pack<?> */ RecursionTail = typename FilterMap<Pack<Tail...>, Mapper>::type;

public:
    using /* Pack<?> */ type = std::conditional_t<
        std::same_as<MappedType, None>,
        RecursionTail,
        prepend_t<MappedType, RecursionTail>
    >;
};

template<
    template<typename> typename Mapper
>
struct FilterMap<Pack<>, Mapper>
{
    using type = Pack<>;
};

template<
    typename Pack,
    template<typename> typename Mapper
>
using filter_map_t = typename FilterMap<Pack, Mapper>::type;

}

#endif // !FILTER_FILTER_MAP_HPP_

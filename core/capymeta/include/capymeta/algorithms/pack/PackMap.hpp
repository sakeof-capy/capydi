#ifndef MAP_PACK_HPP_
#define MAP_PACK_HPP_

#include "capymeta/primitives/Pack.hpp"

namespace capy::meta
{

template<
    typename Pack_, 
    template<typename> typename Mapper
>
struct PackMap;

template<
    template<typename> typename Mapper, 
    typename... PackElements
>
struct PackMap<
    Pack<PackElements...>, 
    Mapper
>
{
    using type = Pack<
        typename Mapper<PackElements>::type...
    >;
};

template<
    template<typename> typename Mapper
>
struct PackMap<
    Pack<>, 
    Mapper
>
{
    using type = Pack<>;
};

template<
    typename Pack_, 
    template<typename> typename Mapper
>
using pack_map_t = typename PackMap<Pack_, Mapper>::type;

}

#endif // !MAP_PACK_HPP_

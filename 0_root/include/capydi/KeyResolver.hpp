#ifndef KEY_RESOLVER_HPP_
#define KEY_RESOLVER_HPP_

#include "utilities/pack/Pack.hpp"
#include "utilities/pack/PackConcat.hpp"

namespace capy::di
{

template<typename... Configs>
struct KeyResolver
{
    using CentralKeys = pack_multy_concat_t<
        typename Configs::ResolutionKeysPack...
    >;
    
    template<typename Key>
    using ResolveKeys = Key;
};

}

#endif // !KEY_RESOLVER_HPP_

#ifndef CAPYDI_CONCAT_HPP_
#define CAPYDI_CONCAT_HPP_

#include "legacy/PackConcat.hpp"

namespace capy::meta
{

template<typename Pack1, typename Pack2>
using pack_concat_t = legacy::pack_concat_t<Pack1, Pack2>;

template<typename... Packs>
using packs_multy_concat_t = legacy::packs_multy_concat_t<Packs...>;

}

#endif //CAPYDI_CONCAT_HPP_
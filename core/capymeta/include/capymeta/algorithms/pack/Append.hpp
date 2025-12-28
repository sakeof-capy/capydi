#ifndef CAPYDI_APPEND_HPP
#define CAPYDI_APPEND_HPP

#include "legacy/Append.hpp"

namespace capy::meta
{

template<typename Element, typename Pack>
using pack_append_t = legacy::append_t<Pack, Element>;

}

#endif //CAPYDI_APPEND_HPP

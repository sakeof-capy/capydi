#ifndef CAPYDI_PREPEND_HPP_
#define CAPYDI_PREPEND_HPP_

#include "legacy/Prepend.hpp"

namespace capy::meta
{

template<typename Pack, typename Element>
using pack_prepend_t = legacy::prepend_t<Pack, Element>;

}

#endif //CAPYDI_PREPEND_HPP_

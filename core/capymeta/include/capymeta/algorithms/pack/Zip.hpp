#ifndef CAPYDI_ZIP_HPP_
#define CAPYDI_ZIP_HPP_

#include "legacy/PackZip.hpp"

namespace capy::meta
{

template<typename Pack1, typename Pack2>
using pack_zip_t = legacy::pack_zip_t<Pack1, Pack2>;

}

#endif //CAPYDI_ZIP_HPP_

#ifndef DI_SOME_CONFIG_HPP_
#define DI_SOME_CONFIG_HPP_

#include "SomeType.hpp"

#include <capydi/configs/creational/Singleton.hpp>
#include <tuple>

std::tuple GENERATE_UNIQUE_NAME = {
    capy::di::Singleton<SomeType>{}
};

#endif // !DI_SOME_CONFIG_HPP_

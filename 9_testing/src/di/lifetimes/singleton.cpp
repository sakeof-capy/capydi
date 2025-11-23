#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include <catch2/catch_test_macros.hpp>

#include "Container.hpp"
#include "configs/creational/Singleton.hpp"

using namespace capy::di;

TEST_CASE("retrieval") {
    const DI container {
    };

    REQUIRE(1 == 2);
}

#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include <catch2/catch_test_macros.hpp>

TEST_CASE("DI") {
    STATIC_CHECK( 1 == 1 );
    STATIC_CHECK( 2 == 2 );
    STATIC_CHECK( 2 == 2 );
}

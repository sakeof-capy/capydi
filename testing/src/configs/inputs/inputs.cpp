#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE

#include <capydi/configs/inputs/TagInput.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace capy::di;

TEST_CASE("inputs")
{
    SECTION("inputs:tag_input")
    {
        TagInput input { "some_tag" };
        
        REQUIRE_FALSE(input.validate());
        REQUIRE(input.tag.value() == "some_tag");
        REQUIRE(input.validate());
    }
}

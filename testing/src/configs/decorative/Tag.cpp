#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include "hierarchies/SpineLeaf3.hpp"

#include <capydi/Container.hpp>
#include <capydi/configs/decorative/Tag.hpp>
#include <capydi/configs/decorative/DependencyTags.hpp>
#include <capydi/configs/creational/Singleton.hpp>
#include <capydi/configs/inputs/TagInput.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace capy::di;

TEST_CASE("singleton/single_tag")
{
    using namespace capy::di::spine_leaf_3;

    const DI container {
        Tag{ 123, Singleton<Leaf1>{} },
        Singleton<Leaf2>{},
        Singleton<Spine1>{},
        DependencyTags { {{ 0, 123 }}, Singleton<Spine2>{} },
        Singleton<RootSpine>{},
    };

    SECTION("leaf_resolution:error_tag")
    {
        Resolution<Leaf1, Error> auto 
            leaf1_resolution_result = container.resolve<Leaf1>();

        REQUIRE_FALSE(leaf1_resolution_result.has_value());
        REQUIRE(leaf1_resolution_result.error() == Error::UNRECOGNIZED_CONFIG_INPUT);
    }

    SECTION("leaf_resolution:good_tag")
    {
        Resolution<Leaf1, Error> auto 
            leaf1_resolution_result = container.resolve<Leaf1>(TagInput {
                123
            });

        REQUIRE(leaf1_resolution_result.has_value());

        Leaf1& leaf1 = leaf1_resolution_result.value();

        REQUIRE(leaf1.get() == Leaf1::IDENTIFIER);
    }

    SECTION("leaf_const_resolution:good_tag")
    {
        Resolution<const Leaf1, Error> auto 
            leaf1_resolution_result = container.resolve<const Leaf1>(TagInput {
                123
            });

        REQUIRE(leaf1_resolution_result.has_value());

        const Leaf1& leaf1 = leaf1_resolution_result.value();

        REQUIRE(leaf1.get() == Leaf1::IDENTIFIER);
    }

    SECTION("spine_resolution:bad_tag")
    {
        Resolution<Spine1, Error> auto 
            spine1_resolution_result = container.resolve<Spine1>();

        REQUIRE_FALSE(spine1_resolution_result.has_value());
        REQUIRE(spine1_resolution_result.error() == Error::DEPENDENCY_CANNOT_BE_RESOLVED); 
        // TODO: return error from config, not from dispatcher (DEPENDENCY_CANNOT_BE_RESOLVED)
    }

    SECTION("spine_resolution:good_tag")
    {
        Resolution<Spine2, Error> auto 
            spine2_resolution_result = container.resolve<Spine2>();

        REQUIRE(spine2_resolution_result.has_value());

        const Spine2& spine2 = spine2_resolution_result.value();

        REQUIRE(spine2.sum() == Spine2::IDENTIFIER);
    }
}

TEST_CASE("singleton/multiple_tags")
{
    using namespace capy::di::spine_leaf_3;

    const DI container {
        Singleton<Leaf1>{},
        Singleton<Leaf2>{},
        Tag { 111, Singleton<Spine1>{} },
        Tag { 222, Singleton<Spine2>{} },
        DependencyTags { {{ 0, 111 }, {1, 222}}, Singleton<RootSpine>{} },
    };

    Resolution<RootSpine, Error> auto 
        root_resolution_result = container.resolve<RootSpine>();

    REQUIRE(root_resolution_result.has_value());

    const RootSpine& root = root_resolution_result.value();

    REQUIRE(root.sum() == RootSpine::IDENTIFIER);
}

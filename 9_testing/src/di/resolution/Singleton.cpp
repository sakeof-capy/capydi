#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include <catch2/catch_test_macros.hpp>

#include "Container.hpp"
#include "configs/creational/Singleton.hpp"
#include "hierarchies/SpineLeaf3.hpp"

using capy::di::DI;
using capy::di::Singleton;


TEST_CASE("singleton:spine_leaf_3") {
    using namespace capy::di::spine_leaf_3;

    const DI container {
        Singleton<Leaf1>{},
        Singleton<Leaf2>{},
        Singleton<Spine1>{},
        Singleton<Spine2>{},
        Singleton<RootSpine>{},
    };

    SECTION("leaf_resolution")
    {
        Leaf1& leaf1 = container.resolve<Leaf1>();
        Leaf2& leaf2 = container.resolve<Leaf2>();

        REQUIRE(leaf1.get() == Leaf1::IDENTIFIER);
        REQUIRE(leaf2.get() == Leaf2::IDENTIFIER);
    }

    SECTION("leaf_const_resolution")
    {
        const Leaf1& leaf1 = container.resolve<const Leaf1>();
        const Leaf2& leaf2 = container.resolve<const Leaf2>();

        REQUIRE(leaf1.get() == Leaf1::IDENTIFIER);
        REQUIRE(leaf2.get() == Leaf2::IDENTIFIER);
    }

    SECTION("spine_resolution")
    {
        Spine1& spine1 = container.resolve<Spine1>();
        Spine2& spine2 = container.resolve<Spine2>();

        REQUIRE(spine1.sum() == Spine1::IDENTIFIER);
        REQUIRE(spine2.sum() == Spine2::IDENTIFIER);
    }

    SECTION("spine_const_resolution")
    {
        const Spine1& spine1 = container.resolve<const Spine1>();
        const Spine2& spine2 = container.resolve<const Spine2>();

        REQUIRE(spine1.sum() == Spine1::IDENTIFIER);
        REQUIRE(spine2.sum() == Spine2::IDENTIFIER);
    }

    SECTION("root_spine_resolution")
    {
        RootSpine& root_spine = container.resolve<RootSpine>();
        REQUIRE(root_spine.sum() == RootSpine::IDENTIFIER);
    }

    SECTION("root_spine_const_resolution")
    {
        const RootSpine& root_spine = container.resolve<const RootSpine>();
        REQUIRE(root_spine.sum() == RootSpine::IDENTIFIER);
    }
}

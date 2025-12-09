#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include <catch2/catch_test_macros.hpp>

#include "Container.hpp"
#include "configs/creational/Singleton.hpp"
#include "hierarchies/SpineLeaf3.hpp"

using capy::di::DI;
using capy::di::Error;
using capy::di::Resolution;
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
        auto leaf1_resolution_result = container.resolve<Leaf1>();
        auto leaf2_resolution_result = container.resolve<Leaf2>();

        REQUIRE(leaf1_resolution_result.has_value());
        REQUIRE(leaf2_resolution_result.has_value());

        Leaf1& leaf1 = leaf1_resolution_result.value();
        Leaf2& leaf2 = leaf2_resolution_result.value();

        REQUIRE(leaf1.get() == Leaf1::IDENTIFIER);
        REQUIRE(leaf2.get() == Leaf2::IDENTIFIER);
    }

    SECTION("leaf_const_resolution")
    {
        auto leaf1_resolution_result = container.resolve<const Leaf1>();
        auto leaf2_resolution_result = container.resolve<const Leaf2>();

        REQUIRE(leaf1_resolution_result.has_value());
        REQUIRE(leaf2_resolution_result.has_value());

        const Leaf1& leaf1 = leaf1_resolution_result.value();
        const Leaf2& leaf2 = leaf2_resolution_result.value();

        REQUIRE(leaf1.get() == Leaf1::IDENTIFIER);
        REQUIRE(leaf2.get() == Leaf2::IDENTIFIER);
    }

    SECTION("spine_resolution")
    {
        auto spine1_resolution_result = container.resolve<Spine1>();
        auto spine2_resolution_result = container.resolve<Spine2>();

        REQUIRE(spine1_resolution_result.has_value());
        REQUIRE(spine2_resolution_result.has_value());

        Spine1& spine1 = spine1_resolution_result.value();
        Spine2& spine2 = spine2_resolution_result.value();

        REQUIRE(spine1.sum() == Spine1::IDENTIFIER);
        REQUIRE(spine2.sum() == Spine2::IDENTIFIER);
    }

    SECTION("spine_const_resolution")
    {
        auto spine1_resolution_result = container.resolve<const Spine1>();
        auto spine2_resolution_result = container.resolve<const Spine2>();

        REQUIRE(spine1_resolution_result.has_value());
        REQUIRE(spine2_resolution_result.has_value());

        const Spine1& spine1 = spine1_resolution_result.value();
        const Spine2& spine2 = spine2_resolution_result.value();

        REQUIRE(spine1.sum() == Spine1::IDENTIFIER);
        REQUIRE(spine2.sum() == Spine2::IDENTIFIER);
    }

    SECTION("root_spine_resolution")
    {
        auto root_spine_resolution_result = container.resolve<RootSpine>();
        REQUIRE(root_spine_resolution_result.has_value());

        RootSpine& root_spine = root_spine_resolution_result.value();
        REQUIRE(root_spine.sum() == RootSpine::IDENTIFIER);
    }

    SECTION("root_spine_const_resolution")
    {
        auto root_spine_resolution_result = container.resolve<const RootSpine>();
        REQUIRE(root_spine_resolution_result.has_value());

        const RootSpine& root_spine = root_spine_resolution_result.value();
        REQUIRE(root_spine.sum() == RootSpine::IDENTIFIER);
    }
}

TEST_CASE("singleton/error_handling:spine_leaf_3") {
    using namespace capy::di::spine_leaf_3;

    SECTION("empty_container_resolution")
    {
        const DI container {
            // Singleton<Leaf1>{},
        };

        Resolution<Leaf1, Error> 
            auto leaf1_resolution_result = container.resolve<Leaf1>(); 

        REQUIRE(!leaf1_resolution_result.has_value());
        REQUIRE(
            leaf1_resolution_result.error() 
            == Error::CANNOT_BE_RESOLVED
        );
    }

    SECTION("absent_dependency_resolution")
    {
        const DI container {
            // Singleton<Leaf1>{},
            Singleton<Leaf2>{},
            Singleton<Spine1>{}
        };

        Resolution<Leaf1, Error> 
            auto leaf1_resolution_result = container.resolve<Leaf1>(); 

        REQUIRE(!leaf1_resolution_result.has_value());
        REQUIRE(leaf1_resolution_result.error() == Error::CANNOT_BE_RESOLVED);
    }

    SECTION("absent_leaf_dependency_resolution_height2")
    {
        const DI container {
            // Singleton<Leaf1>{},
            Singleton<Leaf2>{},
            Singleton<Spine1>{},
            Singleton<Spine2>{},
            Singleton<RootSpine>{},
        };

        Resolution<RootSpine, Error> 
            auto root_spine_resolution_result = container.resolve<RootSpine>(); 

        REQUIRE(!root_spine_resolution_result.has_value());
        REQUIRE(
            root_spine_resolution_result.error() 
            == Error::DEPENDENCY_CANNOT_BE_RESOLVED
        );
    }

    SECTION("absent_spine_dependency_resolution_height2")
    {
        const DI container {
            Singleton<Leaf1>{},
            Singleton<Leaf2>{},
            // Singleton<Spine1>{},
            Singleton<Spine2>{},
            Singleton<RootSpine>{},
        };

        Resolution<RootSpine, Error> 
            auto root_spine_resolution_result = container.resolve<RootSpine>(); 

        REQUIRE(!root_spine_resolution_result.has_value());
        REQUIRE(
            root_spine_resolution_result.error() 
            == Error::DEPENDENCY_CANNOT_BE_RESOLVED
        );
    }
}

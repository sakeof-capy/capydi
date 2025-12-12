#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include <catch2/catch_test_macros.hpp>

#include "hierarchies/ConstSpineLeaf3.hpp"
#include "Container.hpp"
#include "configs/creational/Constleton.hpp"

#include "utilities/referencing/RuntimeRef.hpp"
#include "utilities/pack/Pack.hpp"
#include "dispatchers/CreationalConfigDispatcher.hpp"

using capy::di::DI;
using capy::di::Error;
using capy::di::Resolution;
using capy::di::Constleton;

TEST_CASE("constleton:spine_leaf_3") {
    using namespace capy::di::const_spine_leaf_3;

    static constexpr const DI container {
        Constleton<Leaf1>{},
        Constleton<Leaf2>{},
        Constleton<Spine1>{},
        Constleton<Spine2>{},
        Constleton<RootSpine>{},
    };

    SECTION("leaf_const_resolution")
    {
        static constexpr Resolution<const Leaf1, Error> auto 
            leaf1_resolution_result = container.resolve<const Leaf1>();
        static constexpr Resolution<const Leaf2, Error> auto 
            leaf2_resolution_result = container.resolve<const Leaf2>();

        STATIC_REQUIRE(leaf2_resolution_result.has_value());

        static constexpr const Leaf1& leaf1 = leaf1_resolution_result.value();
        static constexpr const Leaf2& leaf2 = leaf2_resolution_result.value();

        STATIC_REQUIRE(leaf1.get() == Leaf1::IDENTIFIER);
        STATIC_REQUIRE(leaf2.get() == Leaf2::IDENTIFIER);
    }

    SECTION("spine_const_resolution")
    {
        static constexpr Resolution<const Spine1, Error> auto 
            spine1_resolution_result = container.resolve<const Spine1>();
        static constexpr Resolution<const Spine2, Error> auto 
            spine2_resolution_result = container.resolve<const Spine2>();

        STATIC_REQUIRE(spine1_resolution_result.has_value());
        STATIC_REQUIRE(spine2_resolution_result.has_value());

        static constexpr const Spine1& spine1 = spine1_resolution_result.value();
        static constexpr const Spine2& spine2 = spine2_resolution_result.value();

        STATIC_REQUIRE(spine1.sum() == Spine1::IDENTIFIER);
        STATIC_REQUIRE(spine2.sum() == Spine2::IDENTIFIER);
    }

    SECTION("root_spine_const_resolution")
    {
        static constexpr Resolution<const RootSpine, Error> auto 
            root_spine_resolution_result = container.resolve<const RootSpine>();
        STATIC_REQUIRE(root_spine_resolution_result.has_value());

        static constexpr const RootSpine& root_spine = root_spine_resolution_result.value();
        STATIC_REQUIRE(root_spine.sum() == RootSpine::IDENTIFIER);
    }
}

TEST_CASE("constleton/error_handling:spine_leaf_3") {
    using namespace capy::di::const_spine_leaf_3;

    SECTION("absent_dependency_resolution")
    {
        static constexpr DI container {
            // Constleton<Leaf1>{},
            Constleton<Leaf2>{},
            Constleton<Spine1>{}
        };

        static constexpr Resolution<const Leaf1, Error> 
            auto leaf1_resolution_result = container.resolve<const Leaf1>(); 

        STATIC_REQUIRE(!leaf1_resolution_result.has_value());
        STATIC_REQUIRE(leaf1_resolution_result.error() == Error::CANNOT_BE_RESOLVED);
    }

    SECTION("absent_leaf_dependency_resolution_height2")
    {
        static constexpr const DI container {
            // Constleton<Leaf1>{},
            Constleton<Leaf2>{},
            Constleton<Spine1>{},
            Constleton<Spine2>{},
            Constleton<RootSpine>{},
        };

        static constexpr Resolution<const RootSpine, Error> 
            auto root_spine_resolution_result = container.resolve<const RootSpine>(); 

        STATIC_REQUIRE(!root_spine_resolution_result.has_value());
        STATIC_REQUIRE(
            root_spine_resolution_result.error() 
            == Error::DEPENDENCY_CANNOT_BE_RESOLVED
        );
    }

    SECTION("absent_spine_dependency_resolution_height2")
    {
        const DI container {
            Constleton<Leaf1>{},
            Constleton<Leaf2>{},
            // Constleton<Spine1>{},
            Constleton<Spine2>{},
            Constleton<RootSpine>{},
        };

        Resolution<const RootSpine, Error> 
            auto root_spine_resolution_result = container.resolve<const RootSpine>(); 

        REQUIRE(!root_spine_resolution_result.has_value());
        REQUIRE(
            root_spine_resolution_result.error() 
            == Error::DEPENDENCY_CANNOT_BE_RESOLVED
        );
    }
}

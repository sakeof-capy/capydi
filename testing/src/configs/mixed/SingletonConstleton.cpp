#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include "hierarchies/MixedSpineLeaf4.hpp"

#include <capydi/Container.hpp>
#include <capydi/configs/creational/Singleton.hpp>
#include <capydi/configs/creational/Constleton.hpp>
#include <catch2/catch_test_macros.hpp>

using capy::di::DI;
using capy::di::Error;
using capy::di::Resolution;
using capy::di::Singleton;
using capy::di::Constleton;

using capy::di::mixed_spine_leaf_4::TopTopologiesConnector;

TEST_CASE("mix:constleton_singleton") {
    using namespace capy::di;

    SECTION("half_singleton_half_constleton")
    {
        static constexpr const DI container {
            Singleton<spine_leaf_3::Leaf1>{},
            Singleton<spine_leaf_3::Leaf2>{},
            Singleton<spine_leaf_3::Spine1>{},
            Singleton<spine_leaf_3::Spine2>{},
            Singleton<spine_leaf_3::RootSpine>{},

            Constleton<const_spine_leaf_3::Leaf1>{},
            Constleton<const_spine_leaf_3::Leaf2>{},
            Constleton<const_spine_leaf_3::Spine1>{},
            Constleton<const_spine_leaf_3::Spine2>{},
            Constleton<const_spine_leaf_3::RootSpine>{},

            Singleton<TopTopologiesConnector>{}
        };

        Resolution<
            TopTopologiesConnector, 
            Error
        > auto 
            top_root_spine_resolution_result 
                = container.template resolve<TopTopologiesConnector>();

        REQUIRE(top_root_spine_resolution_result.has_value());

        TopTopologiesConnector& root_spine = top_root_spine_resolution_result.value();
        REQUIRE(root_spine.cross_sum() == TopTopologiesConnector::IDENTIFIER);
    }

    SECTION("leaf_not_constleton_error")
    {
        static constexpr const DI container {
            Singleton<spine_leaf_3::Leaf1>{},
            Singleton<spine_leaf_3::Leaf2>{},
            Singleton<spine_leaf_3::Spine1>{},
            Singleton<spine_leaf_3::Spine2>{},
            Singleton<spine_leaf_3::RootSpine>{},

            Singleton<const_spine_leaf_3::Leaf1>{},
            Constleton<const_spine_leaf_3::Leaf2>{},
            Constleton<const_spine_leaf_3::Spine1>{},
            Constleton<const_spine_leaf_3::Spine2>{},
            Constleton<const_spine_leaf_3::RootSpine>{},

            Singleton<TopTopologiesConnector>{}
        };

        Resolution<
            TopTopologiesConnector, 
            Error
        > auto 
            top_root_spine_resolution_result 
                = container.template resolve<TopTopologiesConnector>();

        REQUIRE(!top_root_spine_resolution_result.has_value());
        REQUIRE(
            top_root_spine_resolution_result.error() == 
            Error::DEPENDENCY_CANNOT_BE_RESOLVED
        );
    }

    SECTION("constleton_spine_missing_error")
    {
        static constexpr const DI container {
            Singleton<spine_leaf_3::Leaf1>{},
            Singleton<spine_leaf_3::Leaf2>{},
            Singleton<spine_leaf_3::Spine1>{},
            Singleton<spine_leaf_3::Spine2>{},
            Singleton<spine_leaf_3::RootSpine>{},

            Constleton<const_spine_leaf_3::Leaf1>{},
            Constleton<const_spine_leaf_3::Leaf2>{},
            // Constleton<const_spine_leaf_3::Spine1>{},
            Constleton<const_spine_leaf_3::Spine2>{},
            Constleton<const_spine_leaf_3::RootSpine>{},

            Singleton<TopTopologiesConnector>{}
        };

        Resolution<
            TopTopologiesConnector, 
            Error
        > auto 
            top_root_spine_resolution_result 
                = container.template resolve<TopTopologiesConnector>();

        REQUIRE(!top_root_spine_resolution_result.has_value());
        REQUIRE(
            top_root_spine_resolution_result.error() == 
            Error::DEPENDENCY_CANNOT_BE_RESOLVED
        );
    }
}
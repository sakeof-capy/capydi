// #define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
// #include "hierarchies/SpineLeaf3.hpp"

// #include <capydi/Container.hpp>
// #include <capydi/configs/decorative/Tag.hpp>
// #include <capydi/configs/creational/Singleton.hpp>
// #include <catch2/catch_test_macros.hpp>

// using capy::di::DI;
// using capy::di::Error;
// using capy::di::Resolution;
// using capy::di::Singleton;
// using capy::di::Tag;
// using capy::di::TagType;


// #include <optional>
// template<
//     CreationalConfig Decoratee, 
//     std::optional<std::size_t> Index_ = std::nullopt
// >
// struct DependencyDecorator
// {
// private:
//     using Self = DependencyDecorator<Decorate, Index_>;

// public:
//     template<std::size_t Index>
//     using at = DependencyDecorator<Decoratee, Index>;

//     template<typename DecorativeConfig> 
//         requires requires() { Index_.has_value() }
//     using assign = DecorativeConfig
//         ::template DependencyDecorator<Index_.value(), Self>;
// };

// struct Dependency
// {
//     template<CreationalConfig Decoratee>
//     using Decorator = DependencyDecorator<Decoratee>;
// };

// TEST_CASE("singleton/tag")
// {
//     using namespace capy::di::spine_leaf_3;

//     const DI container {
//         Singleton<Leaf1>
//             ::with<Tag<123>>
//         {},
//         Singleton<Leaf2>{},
//         Singleton<Spine1>
//             ::with<Dependency>
//                 ::at<0>
//                 ::assign<Tag<123>>
//         {},
//         Singleton<Spine2>{},
//         Singleton<RootSpine>{},
//     };

//     SECTION("leaf_resolution")
//     {
//         using Key = capy::di::Pack<Leaf1, capy::di::ValueUnit<capy::di::TagType{123}>>;

//         Resolution<Leaf1, Error> auto 
//             leaf1_resolution_result = container.resolve<Leaf1, Key>();

//         REQUIRE(leaf1_resolution_result.has_value());

//         Leaf1& leaf1 = leaf1_resolution_result.value();

//         REQUIRE(leaf1.get() == Leaf1::IDENTIFIER);
//     }
// }


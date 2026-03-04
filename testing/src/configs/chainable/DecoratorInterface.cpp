#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE

#include "hierarchies/Operations.hpp"

#include <capydi/Container.hpp>
#include <capydi/configs/decorative/Interface.hpp>
#include <capydi/configs/decorative/DependencyTags.hpp>
#include <capydi/configs/chainable/Decorator.hpp>
#include <capydi/configs/creational/Singleton.hpp>
#include <capydi/configs/decorative/Tag.hpp>
#include <capydi/configs/inputs/TagInput.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace capy::di;
using namespace capy::meta;
using namespace capy::di::operations;

TEST_CASE("decorator:as_interface") {
    SECTION("single_decorator")
    {
        const DI container {
            Singleton<Value1>{}
                .with<Interface>(capy::meta::Unit<IValue>{}),
            Decorator<MultiplyBy2_Decorator, IValue>{}
        };

        auto value1_resolution = container.resolve<IValue>();

        REQUIRE(value1_resolution.has_value());

        IValue& value1 = value1_resolution.value();

        REQUIRE(value1.get_value() == Value1::VALUE * 2);
    }

    SECTION("multiple_implementations")
    {
        const DI container {
            Singleton<Value1>{}
                .with<Interface>(Unit<IValue>{})
                .with<Tag>("value1-tag"),
            Singleton<Value3>{}
                .with<Interface>(Unit<IValue>{})
                .with<Tag>("value3-tag"),
            Decorator<MultiplyBy2_Decorator, IValue>{},
            Decorator<MultiplyBy2_Decorator, IValue>{},
        };
        
        auto value1_resolution = container.resolve<IValue>(std::tuple { TagInput {
            "value1-tag"
        }});

        auto value3_resolution = container.resolve<IValue>(std::tuple { TagInput {
            "value3-tag"
        }});

        REQUIRE(value1_resolution.has_value());
        REQUIRE(value3_resolution.has_value());

        IValue& value1 = value1_resolution.value();
        IValue& value3 = value3_resolution.value();

        REQUIRE(value1.get_value() == Value1::VALUE * 2 * 2);
        REQUIRE(value3.get_value() == Value3::VALUE * 2 * 2);

        REQUIRE(std::string { value1.get_name() } == "value1_name");
        REQUIRE(std::string { value3.get_name() } == "value3_name");
    }

    SECTION("decorators_with_tags")
    {
        const DI container {
            Singleton<Value1>{}
                .with<Interface>(Unit<IValue>{})
                .with<Tag>("value1-tag"),
            Singleton<Value3>{}
                .with<Interface>(Unit<IValue>{})
                .with<Tag>("value3-tag"),
            Decorator<MultiplyBy2_Decorator, IValue>{}
                .with<Tag>("value3-tag"),
            Decorator<MultiplyBy2_Decorator, IValue>{},
        };
        
        auto value1_resolution = container.resolve<IValue>(std::tuple { TagInput {
            "value1-tag"
        }});

        auto value3_resolution = container.resolve<IValue>(std::tuple { TagInput {
            "value3-tag"
        }});

        REQUIRE(value1_resolution.has_value());
        REQUIRE(value3_resolution.has_value());

        IValue& value1 = value1_resolution.value();
        IValue& value3 = value3_resolution.value();

        REQUIRE(value1.get_value() == Value1::VALUE * 2);
        REQUIRE(value3.get_value() == Value3::VALUE * 2 * 2);

        REQUIRE(std::string { value1.get_name() } == "value1_name");
        REQUIRE(std::string { value3.get_name() } == "value3_name");
    }

    SECTION("higher_level:decorators_with_tags")
    {
        const DI container {
            Singleton<Value1>{}
                .with<Interface>(Unit<IValue>{})
                .with<Tag>("value1-tag"),
            Singleton<Value3>{}
                .with<Interface>(Unit<IValue>{})
                .with<Tag>("value3-tag"),

            Singleton<Plus>{}
                .with<DependencyTags>(std::array {
                    DependencyTagPair { 0, "value1-tag" },
                    DependencyTagPair { 1, "value3-tag" },
                }),

            Decorator<MultiplyBy2_Decorator, IValue>{},

            Decorator<MultiplyBy2_Decorator, IValue>{}
                .with<Tag>("value1-tag"),

            Decorator<MultiplyBy2_Decorator, IValue>{}
                .with<Tag>("value3-tag"),

            Decorator<MultiplyBy2_Decorator, IValue>{}
                .with<Tag>("value3-tag"),
        };

        Resolution<Plus, Error> auto plus_resolution = container.resolve<Plus>();

        REQUIRE(plus_resolution.has_value());
        Plus& plus = plus_resolution.value();

        REQUIRE(plus.evaluate() == Value1::VALUE * 2 * 2 + Value3::VALUE * 2 * 2 * 2);
    }
}

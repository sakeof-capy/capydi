#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE

#include <capydi/Container.hpp>
#include <capydi/configs/decorative/Interface.hpp>
#include <capydi/configs/chainable/Decorator.hpp>
#include <capydi/configs/creational/Singleton.hpp>
#include <capydi/configs/decorative/Tag.hpp>
#include <capydi/configs/inputs/TagInput.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace capy::di;
using namespace capy::meta;

class IValue
{
public:
    virtual ~IValue() = default;

public:
    virtual int get_value() = 0;
    virtual const char* get_name() const = 0;
};

class Value1 : public IValue
{
public:
    static constexpr int VALUE = 1;

public:
    int get_value() override
    {
        return VALUE;
    }

    const char* get_name() const override
    {
        return "value1_name";
    }

public:
    static Value1 create()
    {
        return Value1{};
    }
};

class Value3 : public IValue
{
public:
    static constexpr int VALUE = 3;

public:
    int get_value() override
    {
        return VALUE;
    }

    const char* get_name() const override
    {
        return "value3_name";
    }

public:
    static Value3 create()
    {
        return Value3{};
    }
};

class MultiplyBy2_Decorator : public IValue
{
public:
    explicit MultiplyBy2_Decorator(IValue& decoratee)
        : decoratee_ { decoratee }
    {}

public:
    int get_value() override
    {
        return 2 * decoratee_.get_value();
    }

    const char* get_name() const override
    {
        return decoratee_.get_name();
    }

public:
    static MultiplyBy2_Decorator create(IValue& decoratee)
    {
        return MultiplyBy2_Decorator { decoratee };
    }

private:
    IValue& decoratee_;
};

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
}

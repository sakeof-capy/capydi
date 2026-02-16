#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE

#include <capydi/Container.hpp>
#include <capydi/configs/decorative/Interface.hpp>
#include <capydi/configs/chainable/Decorator.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace capy::di;

class IValue
{
public:
    virtual ~IValue() = default;

public:
    virtual int get_value() = 0;
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

public:
    static MultiplyBy2_Decorator create(IValue& decoratee)
    {
        return MultiplyBy2_Decorator { decoratee };
    }

private:
    IValue& decoratee_;
};

TEST_CASE("decorator:as_interface") {
    const DI container {
        AsInterface { capy::meta::Unit<IValue>{}, Singleton<Value1>{} },
        // AsInterface { capy::meta::Unit<IValue>{}, Singleton<Value3>{} },
        Decorator<MultiplyBy2_Decorator, IValue>{}
    };

    auto value1_resolution = container.resolve<IValue>();

    REQUIRE(value1_resolution.has_value());

    IValue& value1 = value1_resolution.value();

    REQUIRE(value1.get_value() == Value1::VALUE * 2);
}

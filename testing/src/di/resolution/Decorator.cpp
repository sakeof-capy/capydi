#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include "hierarchies/SpineLeaf3.hpp"

#include <capydi/Container.hpp>
#include <capydi/configs/chainable/Decorator.hpp>
#include <catch2/catch_test_macros.hpp>

using capy::di::DI;
using capy::di::Error;
using capy::di::Resolution;
using capy::di::Singleton;
using capy::di::Decorator;


using namespace capy::di::spine_leaf_3;

class Leaf1Decorator1 : public Leaf1
{
public:
    static constexpr int IDENTIFIER = 89023;

public:
    explicit Leaf1Decorator1(Leaf1& leaf) : leaf_(leaf) {}

    ~Leaf1Decorator1() override = default;

    static Leaf1Decorator1 create(Leaf1& leaf)
    {
        return Leaf1Decorator1(leaf);
    }

    virtual int get() const
    {
        return leaf_.get() + IDENTIFIER;
    }

private:
    Leaf1& leaf_;
};

class Leaf1Decorator2 : public Leaf1
{
public:
    static constexpr int IDENTIFIER = 34123;

public:
    explicit Leaf1Decorator2(Leaf1& leaf) : leaf_{leaf} {}

    ~Leaf1Decorator2() override = default;

    static Leaf1Decorator2 create(Leaf1& leaf)
    {
        return Leaf1Decorator2(leaf);
    }

    virtual int get() const
    {
        return leaf_.get() + IDENTIFIER;
    }

private:
    Leaf1& leaf_;
};

TEST_CASE("DI") {
    const DI container {
        Singleton<Leaf1>{},
        Decorator<Leaf1Decorator1, Leaf1>{},
        Decorator<Leaf1Decorator2, Leaf1>{}
    };

    auto leaf1_resolution_result = container.resolve<Leaf1>();

    REQUIRE(leaf1_resolution_result.has_value());

    Leaf1& ref = leaf1_resolution_result.value();

    REQUIRE(ref.get() == Leaf1::IDENTIFIER 
        + Leaf1Decorator1::IDENTIFIER 
        + Leaf1Decorator2::IDENTIFIER);
}

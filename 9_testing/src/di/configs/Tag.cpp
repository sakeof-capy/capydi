#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include <catch2/catch_test_macros.hpp>

#include "Container.hpp"
#include "configs/creational/Singleton.hpp"
#include "configs/decorative/Tag.hpp"

using namespace capy::di;

struct A 
{
    constexpr static A create() noexcept {
        return A{};
    }
};


struct M1
{
    template<typename... Deps>
    int m(Pack<int>, std::tuple<Deps...>& t)
    {
        return 0;
    }
};

struct M2
{
    template<typename... Deps>
    int m(Pack<const int>, std::tuple<Deps...>& t)
    {
        return 1;
    }
};

template<typename... Configs>
struct MM : private Configs...
{
public:
    using Configs::do_resolve...;
    
public:
    constexpr explicit MM(Configs&&... configs) 
        : Configs(std::move(configs))...
    {}
};

TEST_CASE("Tag") {
    const DI container {
        Singleton<A>::with<Tag<2>>{}
    };

    A& a_ref = container.resolve<A, TagKey<2>>();
    
}

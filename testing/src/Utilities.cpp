#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE

#include "capydi/utilities/MetaFunctor.hpp"
#include "capydi/utilities/pack/Filter.hpp"
#include "capydi/utilities/pack/PackMap.hpp"
#include "capydi/utilities/Overload.hpp"

#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <utility>

TEST_CASE("utilities")
{
    using namespace capy::di;

    SECTION("meta_predicate:filter")
    {
        using In = Pack<int, float, int, double, int>;
        using Output = filter_t<
            In, 
            UnaryPredicate<[]<typename T>(Unit<T>&&) {
                return !std::same_as<T, int>;
            }>::template F
        >;

        STATIC_REQUIRE(std::same_as<Output, Pack<float, double>>);
    }

    SECTION("meta_functor:pack_map")
    {
        using In = Pack<int, float, int, double, int>;
        using Output = pack_map_t<
            In, 
            MetaUnary<
                Overload {
                    []<typename T>(Unit<T>&&) -> ValueUnit<T{}> { 
                        std::unreachable(); 
                    },
                    [](Unit<int>) -> ValueUnit<1> { 
                        std::unreachable();
                    }
                }
            >::template F
        >;

        STATIC_REQUIRE(std::same_as<
            Output, 
            Pack<
                ValueUnit<1>, 
                ValueUnit<0.0f>, 
                ValueUnit<1>, 
                ValueUnit<0.0>, 
                ValueUnit<1>
            >
        >);
    }
}

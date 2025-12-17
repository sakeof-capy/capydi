#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE

#include "capymeta/MetaFunctor.hpp"
#include "capymeta/pack/Filter.hpp"
#include "capymeta/pack/PackMap.hpp"
#include "capymeta/pack/Append.hpp"
#include "capymeta/Overload.hpp"
#include "capymeta/pack/Maybe.hpp"

#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <utility>

using namespace capy::meta;

TEST_CASE("utilities:meta_functors")
{
    
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

template<typename T>
concept Innerable = requires() {
    typename T::Inner;
};

template<typename T>
struct GetInner
{
    using type = None;
};

template<Innerable T>
struct GetInner<T>
{
    using type = Some<typename T::Inner>;
};

template<typename T>
using get_inner_t = typename GetInner<T>::type;

TEST_CASE("utilities:meta_optional")
{
    SECTION("maybe:value")
    {
        struct WithInner
        {
            using Inner = int;
        };

        struct NoInner{};

        STATIC_REQUIRE(get_inner_t<WithInner>::HAS_VALUE);
        STATIC_REQUIRE(std::same_as<get_inner_t<WithInner>::Value, int>);

        STATIC_REQUIRE(!get_inner_t<NoInner>::HAS_VALUE);
    }

    SECTION("maybe:map")
    {
        struct WithInner
        {
            using Inner = int;
        };

        struct NoInner{};

        using MaybeChain = 
            get_inner_t<WithInner>
            ::template Map<MetaUnary<[]<class T>(Unit<T>) {
                return Pack<T, float>{};
            }>::template F>
            ::template Map<MetaUnary<[]<class T>(Unit<T>) {
                return append_t<double, T>{};
            }>::template F>;

        using MaybeNoChain = 
            get_inner_t<NoInner>
            ::template Map<MetaUnary<[]<class T>(Unit<T>) {
                return Pack<T, float>{};
            }>::template F>
            ::template Map<MetaUnary<[]<class T>(Unit<T>) {
                return append_t<double, T>{};
            }>::template F>;

        STATIC_REQUIRE(std::same_as<MaybeChain, Some<Pack<int, float, double>>>);
        STATIC_REQUIRE(!MaybeNoChain::HAS_VALUE);
    }

    SECTION("maybe:or")
    {
        struct WithInner
        {
            using Inner = int;
        };

        struct NoInner{};

        using MaybeWithInner = get_inner_t<WithInner>::Or<std::size_t>;
        using MaybeNoInner = get_inner_t<NoInner>::Or<std::size_t>;

        STATIC_REQUIRE(std::same_as<MaybeWithInner, Some<int>>);
        STATIC_REQUIRE(std::same_as<MaybeNoInner, Some<std::size_t>>);
    }

    SECTION("maybe:filter")
    {
        struct WithInner
        {
            using Inner = int;
        };

        struct NoInner{};

        using NotFiltered = 
            get_inner_t<WithInner>
            ::Filter<UnaryPredicate<[]<class T>(Unit<T>) {
                return std::same_as<T, int>;
            }>::template F>;

        using FilteredOut = 
            get_inner_t<WithInner>
            ::Filter<UnaryPredicate<[]<class T>(Unit<T>) {
                return !std::same_as<T, int>;
            }>::template F>;

        STATIC_REQUIRE(std::same_as<NotFiltered, Some<int>>);
        STATIC_REQUIRE(!FilteredOut::HAS_VALUE);
    }
}

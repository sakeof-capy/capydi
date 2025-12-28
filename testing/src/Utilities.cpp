#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE

#include "capymeta/primitives/Template.hpp"
#include "capymeta/primitives/Functor.hpp"
#include "capymeta/algorithms/pack/Filter.hpp"
#include "capymeta/algorithms/pack/Map.hpp"
#include "capymeta/algorithms/pack/Append.hpp"
#include "capymeta/primitives/Overload.hpp"
#include "capymeta/type_structures/Maybe.hpp"

#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <utility>

using namespace capy::meta;

template<typename T, typename T2, typename...>
struct Pred
{
    static constexpr bool value = std::same_as<T, int>;
};

template<typename T, typename T2>
struct Pred2
{
    static constexpr bool value = std::same_as<T, int>;
};

TEST_CASE("utilities:meta_functors")
{
    SECTION("template_matchers:unify")
    {
        using f = template_fv<Pred, MetaArity::N3>;
        STATIC_REQUIRE(call_v<f, int, double, float>);
        STATIC_REQUIRE(!call_v<f, double, double, float>);

        using f2 = template_fv<Pred, MetaArity::N3>;
        STATIC_REQUIRE(call_v<f2, int, double, float>);
        STATIC_REQUIRE(!call_v<f2, double, double, float>);

        using f3 = template_fv<Pred2, MetaArity::N2>;
        STATIC_REQUIRE(call_v<f3, int, double>);
        STATIC_REQUIRE(!call_v<f3, double, double>);

        using f4 = functor_fv<[]<typename T>(Pack<T>) { 
            return std::same_as<T, int>; 
        }, MetaArity::N1>;

        STATIC_REQUIRE(call_v<f4, int>);
        STATIC_REQUIRE(!call_v<f4, double>);
        
        STATIC_REQUIRE(call_v<f4, int>);
        STATIC_REQUIRE(!call_v<f4, double>);
    }

    SECTION("meta_predicate:filter")
    {
        using In = Pack<int, float, int, double, int>;
        using Output = pack_filter_t<
            In, 
            functor_fv<[]<typename T>(Pack<T>&&) {
                return !std::same_as<T, int>;
            }, MetaArity::N1>
        >;

        STATIC_REQUIRE(std::same_as<Output, Pack<float, double>>);
    }

    SECTION("meta_functor:pack_map")
    {
        using In = Pack<int, float, int, double, int>;
        using Output = pack_map_t<
            In, 
            functor_ft<
                Overload {
                    []<typename T>(Pack<T>&&) -> ValueUnit<T{}> { 
                        return {}; 
                    },
                    [](Pack<int>) -> ValueUnit<1> { 
                        return {};
                    }
                },
                MetaArity::N1
            >
        >;

        static_assert(std::same_as<
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
            ::template Map<functor_ft<[]<class T>(Pack<T>) {
                return Pack<T, float>{};
            }, MetaArity::N1>>
            ::template Map<functor_ft<[]<class T>(Pack<T>) {
                return pack_append_t<T, double>{};
            }, MetaArity::N1>>;

        using MaybeNoChain = 
            get_inner_t<NoInner>
            ::template Map<functor_ft<[]<class T>(Pack<T>) {
                return Pack<T, float>{};
            }, MetaArity::N1>>
            ::template Map<functor_ft<[]<class T>(Pack<T>) {
                return legacy::append_t<T, double>{};
            }, MetaArity::N1>>;

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
            ::Filter<functor_fv<[]<class T>(Pack<T>) {
                return std::same_as<T, int>;
            }, MetaArity::N1>>;

        using FilteredOut = 
            get_inner_t<WithInner>
            ::Filter<functor_fv<[]<class T>(Pack<T>) {
                return !std::same_as<T, int>;
            }, MetaArity::N1>>;

        STATIC_REQUIRE(std::same_as<NotFiltered, Some<int>>);
        STATIC_REQUIRE(!FilteredOut::HAS_VALUE);
    }
}

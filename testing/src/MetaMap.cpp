#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE

#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <utility>
#include <type_traits>
#include <tuple>

#include <boost/mp11.hpp>
#include <capymeta/type_structures/Maybe.hpp>

using namespace capy;

using namespace boost::mp11;

template<typename... Configs>
struct A
{
    template<typename UniqueType, typename... Args>
    static constexpr auto collect(Args&&... args)
    {
        auto filtered = std::tuple_cat(
            ([&]() {
                if constexpr (std::is_same_v<UniqueType, std::decay_t<Args>>)
                    return std::tuple<UniqueType>{ std::forward<Args>(args) };
                else
                    return std::tuple<>{};
            }())...
        );

        return std::apply(
            []<typename... Filtered>(Filtered&&... values)
            {
                return std::array<UniqueType, sizeof...(Filtered)>{
                    std::forward<Filtered>(values)...
                };
            },
            std::move(filtered)
        );
    }



    static constexpr auto populate_configs_map(Configs&&... configs)
    {
        using list = mp_list<std::decay_t<Configs>...>;
        using unique_types = mp_unique<list>;

        return [&]<typename... UniqueTypes>(mp_list<UniqueTypes...>&& list) {
            return std::tuple{
                collect<UniqueTypes>(
                    std::move(configs)...
                )...
            };
        }(unique_types{});
    }
};

TEST_CASE("multy_meta_map")
{
    auto a = A<int, float, double, float, double, float>{};
    auto classified = a.populate_configs_map(1, 2.0f, 3.0, 4.0f, 5.0, 6.0f);

    // static_assert(std::same_as<decltype(classified),
    //     std::tuple<
            
    //     >
    // >);
}

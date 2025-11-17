#ifndef PACK_ALGORITHM_HPP_
#define PACK_ALGORITHM_HPP_

#include "Pack.hpp"

#include <tuple>
#include <type_traits>
#include <functional>

namespace capy::di
{

namespace hidden__
{
    template<typename Handler, typename... PackElements>
    using valued_pack_for_result = std::tuple<
        std::invoke_result_t<Handler, Unit<PackElements>>...
    >;

    template<
        typename Handler, 
        typename AccumulatedResult
    >
    constexpr inline AccumulatedResult valued_pack_for_utility__(
        Pack<>&& pack, 
        Handler&& handler,
        AccumulatedResult&& accumulated_result
    ) {
        return std::move(accumulated_result);
    }

    template<
        typename Handler, 
        typename AccumulatedResult, 
        typename Head, typename... Tail
    >
    constexpr inline auto valued_pack_for_utility__(
        Pack<Head, Tail...>&& pack, 
        Handler&& handler,
        AccumulatedResult&& accumulated_result
    ) {
        auto next_accumulated_result = std::tuple_cat(
            std::forward<AccumulatedResult>(accumulated_result), 
            std::tuple<Head> { std::invoke(handler, Unit<Head>{}) }
        );

        return valued_pack_for_utility__(
            Pack<Tail...>{},
            std::forward<Handler>(handler),
            std::move(next_accumulated_result)
        );
    }
}

template<typename Handler, typename... PackElements>
constexpr inline hidden__::valued_pack_for_result<Handler, PackElements...> valued_pack_for(
    Pack<PackElements...>&& pack, 
    Handler&& handler
) {
    return hidden__::valued_pack_for_utility__(
        std::move(pack),
        std::forward<Handler>(handler),
        std::tuple{}
    );
}

}

#endif // !PACK_ALGORITHM_HPP_

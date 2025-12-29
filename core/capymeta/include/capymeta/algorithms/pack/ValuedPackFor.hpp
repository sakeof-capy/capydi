#ifndef PACK_ALGORITHM_HPP_
#define PACK_ALGORITHM_HPP_

#include "capymeta/primitives/Pack.hpp"
#include "legacy/PackMap.hpp"
#include "Head.hpp"
#include "legacy/PackConcat.hpp"
#include "capymeta/algorithms/general/Rebind.hpp"

#include <tuple>
#include <type_traits>
#include <functional>
#include <expected>

namespace capy::meta
{

/// @cond HIDDEN

namespace implementation_details_
{
    template<typename... Types>
    struct AllSame;

    template<typename Head, typename... Tail>
    struct AllSame<Head, Tail...> 
        : std::conjunction<std::is_same<Head, Tail>...> 
    {};

    template<>
    struct AllSame<> 
        : std::true_type 
    {};

    template<typename... Types>
    using all_same_v = AllSame<Types...>::value;

    template<typename Handler, typename... PackElements>
    struct ValuedPackForResult
    {
    private:
        template<typename Expected>
        struct ValueTypeMapper
        {
            using type = typename Expected::value_type;
        };

        template<typename Expected>
        struct ErrorTypeMapper
        {
            using type = typename Expected::error_type;
        };

        using ResultsPack = Pack<
            typename std::invoke_result_t<
                Handler, 
                Unit<PackElements>
            >...
        >;

        using SuccessSubtypesPack = legacy::pack_map_t<
            ResultsPack,
            ValueTypeMapper
        >;

        using ErrorSubtypesPack = legacy::pack_map_t<
            ResultsPack,
            ErrorTypeMapper
        >;

        static constexpr bool all_error_types_same = 
            rebind_pack_t<ErrorSubtypesPack, AllSame>::value;

        static_assert(all_error_types_same, "All error types must be equal");

        using SuccessType = rebind_pack_t<SuccessSubtypesPack, std::tuple>;
        using ErrorType = pack_head_t<ErrorSubtypesPack>::template Or<None>;

    public:
        using type = std::expected<SuccessType, ErrorType>;
    };

    template<typename Handler, typename... PackElements>
    using valued_pack_for_result_t = typename ValuedPackForResult<
        Handler, 
        PackElements...
    >::type;

    template<
        typename Handler, 
        typename AccumulatedResult, 
        typename ErrorType,
        typename TypesPack
    >
    struct ValuedPackForUtilityResult;

    template<
        typename Handler, 
        typename AccumulatedResult, 
        typename ErrorType_,
        typename... Types
    >
    struct ValuedPackForUtilityResult
        <
            Handler,
            AccumulatedResult,
            ErrorType_,
            Pack<Types...>
        >
    {
    private:
        template<typename Expected>
        struct ValueTypeMapper
        {
            using type = typename Expected::value_type;
        };

        using AccumulatedResultPack = rebind_t<
            AccumulatedResult, 
            std::tuple, 
            Pack    
        >;
        
        using NextTypesPack = Pack<
            typename std::invoke_result_t<
                Handler, 
                Unit<Types>
            >...
        >;

        using NextSuccessTypesPack = legacy::pack_map_t<
            NextTypesPack,
            ValueTypeMapper
        >;

        using SuccessSubtypesPack = legacy::pack_concat_t<
            AccumulatedResultPack, 
            NextSuccessTypesPack
        >;

        using SuccessType = rebind_pack_t<SuccessSubtypesPack, std::tuple>;
        using ErrorType = ErrorType_; // None if empty

    public:
        using type = std::expected<SuccessType, ErrorType>;
    };

    template<
        typename Handler, 
        typename AccumulatedResult, 
        typename ErrorType,
        typename TypesPack
    >
    using valued_pack_for_utility_result_t = typename ValuedPackForUtilityResult<
        Handler,
        AccumulatedResult,
        ErrorType,
        TypesPack
    >::type;

    template<
        typename Handler, 
        typename AccumulatedResult,
        typename ErrorType
    >
    constexpr inline std::expected<AccumulatedResult, ErrorType> valued_pack_for_utility__(
        Pack<>&& pack, 
        Handler&& handler,
        AccumulatedResult&& accumulated_result,
        Unit<ErrorType> error_type
    ) {
        return std::forward<AccumulatedResult>(accumulated_result);
    }

    template<
        typename Handler, 
        typename AccumulatedResult, 
        typename ErrorType,
        typename Head, typename... Tail
    >
    constexpr inline valued_pack_for_utility_result_t<
        Handler,
        AccumulatedResult,
        ErrorType,
        Pack<Head, Tail...>
    > valued_pack_for_utility__(
        Pack<Head, Tail...>&& pack, 
        Handler&& handler,
        AccumulatedResult&& accumulated_result,
        Unit<ErrorType>&& error_type
    ) {
        auto /* std::expected */ handler_result = 
            std::invoke(handler, Unit<Head>{});

        if (!handler_result.has_value()) [[unlikely]]
        {
            return std::unexpected(handler_result.error());
        }

        auto successful_handling_result = handler_result.value();

        auto next_accumulated_result = std::tuple_cat(
            std::forward<AccumulatedResult>(accumulated_result), 
            std::tuple { successful_handling_result }
        );

        return valued_pack_for_utility__(
            Pack<Tail...>{},
            std::forward<Handler>(handler),
            std::move(next_accumulated_result),
            std::move(error_type)
        );
    }
}

//// @endcond

template<typename Handler, typename... PackElements>
constexpr implementation_details_::valued_pack_for_result_t<
    Handler, PackElements...
> valued_pack_for(
    Pack<PackElements...>&& pack, 
    Handler&& handler
) {
    using ReturnType = decltype(valued_pack_for(
        std::move(pack), 
        std::forward<Handler>(handler)
    ));

    return implementation_details_::valued_pack_for_utility__(
        std::move(pack),
        std::forward<Handler>(handler),
        std::tuple{},
        Unit<typename ReturnType::error_type>{}
    );
}

}

#endif // !PACK_ALGORITHM_HPP_

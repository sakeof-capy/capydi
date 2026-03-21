#ifndef CAPYDI_DI_ACTION_HPP_
#define CAPYDI_DI_ACTION_HPP_

#include "capydi/configs/decorative/DependencyTags.hpp"
#include "capydi/ResolutionContext.hpp"

#include <capymeta/concepts/WrappedWIth.hpp>
#include <capymeta/algorithms/pack/legacy/FunctionTraits.hpp>
#include <expected>
#include <array>
#include <ranges>

namespace capy::di
{

template<
    typename ActionFunctor, 
    std::size_t DEPENDENCY_TAGS_COUNT
>
struct DiAction
{
public:
    using ReturnType 
        = meta::return_value_t<decltype(&ActionFunctor::operator())>;

    using ArgsPack 
        = meta::args_pack_t<decltype(&ActionFunctor::operator())>;

public:
    ActionFunctor action;

    std::array<
        DependencyTagPair, 
        DEPENDENCY_TAGS_COUNT
    > dependency_tags;
};

template<typename T>
concept MaybeLike = requires (T maybe) {
    { maybe.has_value() } -> std::convertible_to<bool>;
    { maybe.value() };
    typename T::value_type;
};

template<typename DiAction_>
using di_action_return_t = typename DiAction_::ReturnType;

template<typename DiAction_>
using di_action_args_pack_t = typename DiAction_::ArgsPack;

enum class DiActionError
{
    DI_ACTION_UNABLE_TO_RESOLVE_ARGS,
    DI_ACTION_EXECUTION_ERROR,
};

template<typename DiAction_>
inline auto execute_di_action(
    DiAction_ const& di_action,
    auto& context
) {
    using ActionReturnType = di_action_return_t<DiAction_>;
    using ActionDependenciesPack = di_action_args_pack_t<DiAction_>;

    using ResultType = meta::unit_inner_type_t<decltype([] {
        if constexpr (MaybeLike<ActionReturnType>)
        {
            return meta::Unit<
                std::expected<
                    typename ActionReturnType::value_type,
                    DiActionError
                >
            >{};
        }
        else
        {
            return meta::Unit<
                std::expected<
                    ActionReturnType,
                    DiActionError
                >
            >{};
        }
    }())>;

    auto maybe_args = [di_action, &context]<typename... ActionDependencies>(
        meta::Pack<ActionDependencies...>
    ) {
        auto args_preparations = [di_action, &context]<std::size_t... Idx>(std::index_sequence<Idx...>) {
            return std::tuple {
                ActionArgMatcher<ActionDependencies>::prepare_arg(
                    context, 
                    [di_action]() -> std::optional<tag_t> {
                        auto it = std::ranges::find_if(
                            di_action.dependency_tags, 
                            [](const auto& tag_pair){
                                return tag_pair.first == Idx;
                            }
                        );

                        if (it == std::ranges::end(di_action.dependency_tags)) [[unlikely]]
                        {
                            return std::nullopt;
                        }

                        return it->second;
                    }()
                )...
            };
        }(std::index_sequence_for<ActionDependencies...>{});

        return std::apply([](auto&&... preparations) {
            using TupleResult = std::tuple<ActionDependencies...>;

            if ((preparations.has_value() && ...))
            {
                return std::expected<TupleResult, Error> {
                    std::tuple { std::move(preparations.value())... }
                };
            }

            return std::expected<TupleResult, Error> {
                std::unexpected { Error::OBSERVER_COULD_NOT_RESOLVE_TYPE }
            };
        }, std::move(args_preparations));
    }(ActionDependenciesPack{});

    if (!maybe_args.has_value()) [[unlikely]]
    {
        return ResultType { std::unexpected { 
            DiActionError::DI_ACTION_UNABLE_TO_RESOLVE_ARGS
        }};
    }

    auto& args = maybe_args.value();

    if constexpr (MaybeLike<ActionReturnType>)
    {
        ActionReturnType action_result 
            = std::apply(di_action.action, args);

        if (!action_result.has_value()) [[unlikely]]
        {
            return ResultType { std::unexpected {
                DiActionError::DI_ACTION_EXECUTION_ERROR
            }};
        }

        return ResultType { action_result.value() };
    }
    else if constexpr (std::is_void_v<ActionReturnType>)
    {
        std::apply(di_action.action, args);
        return ResultType {};
    }
    else
    {   
        ActionReturnType action_result 
            = std::apply(di_action.action, args);

        return ResultType { action_result };
    }
}


}

#endif // !CAPYDI_DI_ACTION_HPP_

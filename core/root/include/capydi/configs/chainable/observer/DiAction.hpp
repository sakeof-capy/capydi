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
    ActionFunctor action;

    std::array<
        DependencyTagPair, 
        DEPENDENCY_TAGS_COUNT
    > dependency_tags;
};

enum class DiActionError
{
    DI_ACTION_UNABLE_TO_RESOLVE_ARGS,
    DI_ACTION_EXECUTION_ERROR,
};

inline std::expected<void, DiActionError> execute_di_action(
    /*meta::wrapped_with<DiAction>*/ auto const& di_action,
    /*meta::wrapped_with<ResolutionContext>*/ auto& context
)
{
    using Action = decltype(di_action.action);
    using ActionReturnType 
        = meta::return_value_t<decltype(&Action::operator())>;
    using ActionDependenciesPack 
        = meta::args_pack_t<decltype(&Action::operator())>;

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
        return std::unexpected { 
            DiActionError::DI_ACTION_UNABLE_TO_RESOLVE_ARGS
        };
    }

    auto& args = maybe_args.value();

    if constexpr (std::is_void_v<ActionReturnType>)
    {
        std::apply(di_action.action, args);
        return {};
    }
    else 
    {
        ActionReturnType action_result 
            = std::apply(di_action.action, args);
        
        if (!action_result) [[unlikely]]
        {
            return std::unexpected {
                DiActionError::DI_ACTION_EXECUTION_ERROR
            };
        }

        return {};
    }
}


}

#endif // !CAPYDI_DI_ACTION_HPP_

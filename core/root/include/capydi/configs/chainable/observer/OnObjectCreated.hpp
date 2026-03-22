#ifndef CAPYDI_ON_OBJECT_CREATED_HPP_
#define CAPYDI_ON_OBJECT_CREATED_HPP_

#include "ActionArgMatcher.hpp"
#include "DiAction.hpp"

#include "capydi/Resolution.hpp"
#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/configs/decorative/DependencyTags.hpp"
#include "capydi/ResolutionContext.hpp"
#include "capydi/Error.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/concepts/WrappedWIth.hpp>
#include <optional>

namespace capy::di
{

template<
    typename RelatedEntity_,
    typename Action,
    std::size_t SIZE
>
class OnObjectCreated 
    : public DecoratableChainableConfig<
        OnObjectCreated<RelatedEntity_, Action, SIZE>
    >
{
public:
    using RelatedEntity = RelatedEntity_;
    using RelatedKeysPack = meta::Pack<meta::Pack<RelatedEntity_>>;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CHAINABLE;

public:
    constexpr explicit OnObjectCreated(
        meta::Unit<RelatedEntity_>,
        DiAction<Action, SIZE>&& action
    )
        : di_action_ { std::move(action) } 
    {}

public:
    Resolution<RelatedEntity, Error> auto
        pipe(
            meta::Reference<RelatedEntity> auto decoratee,
            meta::wrapped_with<ResolutionContext> auto& context
        ) const
    {
        if (!context.flags.just_created) [[unlikely]]
        {
            return std::expected<decltype(decoratee), Error> {
                decoratee
            };
        }

        return execute_di_action(this->di_action_, context)
            .transform([&] {
                return decoratee;
            })
            .transform_error([](DiActionError action_error) {
                switch (action_error)
                {
                    case DiActionError::DI_ACTION_UNABLE_TO_RESOLVE_ARGS:
                        return Error::OBSERVER_COULD_NOT_RESOLVE_TYPE;
                    case DiActionError::DI_ACTION_EXECUTION_ERROR:
                        return Error::OBSERVER_ACTION_EXECUTION_FAILURE;
                    default:
                        std::unreachable();
                }
            });
    }

private:
    DiAction<Action, SIZE> di_action_;
};

}

#endif // !CAPYDI_ON_OBJECT_CREATED_HPP_

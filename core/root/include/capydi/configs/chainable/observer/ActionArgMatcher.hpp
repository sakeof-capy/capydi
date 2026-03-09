#ifndef CAPYDI_ACTION_ARG_MATCHER_HPP_
#define CAPYDI_ACTION_ARG_MATCHER_HPP_

#include "DynamicResolver.hpp"
#include "capydi/configs/inputs/TagInput.hpp"

#include <capymeta/concepts/WrappedWIth.hpp>
#include <tuple>

namespace capy::di
{

template<typename ActionArg>
struct ActionArgMatcher;

template<typename Type>
struct ActionArgMatcher<Type&>
{
    static std::expected<meta::RuntimeRef<Type>, Error> prepare_arg(
        meta::wrapped_with<ResolutionContext> auto& context,
        std::optional<tag_t> tag
    ) {
        if (tag.has_value())
        {
            auto resolution = context
                .container
                .template resolve<Type>(std::tuple { TagInput {
                    tag.value()
                }});

            return resolution;
        }
        else
        {
            auto resolution = context
                .container
                .template resolve<Type>();

            return resolution;
        }
    }
};

template<typename Type>
struct ActionArgMatcher<DynamicResolver<Type>>
{
    static std::expected<DynamicResolver<Type>, Error> prepare_arg(
        meta::wrapped_with<ResolutionContext> auto& context,
        std::optional<tag_t> tag
    ) {

        if (tag.has_value())
        {
            return DynamicResolver<Type> { 
                context,
                tag.value()
            };
        }
        else
        {
            return DynamicResolver<Type> { 
                context,
            }; 
        }
    }
};

}

#endif // !CAPYDI_ACTION_ARG_MATCHER_HPP_

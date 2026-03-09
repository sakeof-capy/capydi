#ifndef CAPYDI_DYNAMIC_RESOLVER_HPP_
#define CAPYDI_DYNAMIC_RESOLVER_HPP_

#include "capydi/configs/inputs/TagInput.hpp"

#include <functional>

namespace capy::di
{

template<typename Type>
class DynamicResolver
{
public:
    using ReturnType = std::expected<meta::RuntimeRef<Type>, Error>;

public:
    explicit DynamicResolver(
        meta::wrapped_with<ResolutionContext> auto const& context
    )
        : resolver_ { [&context] {
            return context
                .container
                .template resolve<Type>();
        }}
    {}

    DynamicResolver(
        meta::wrapped_with<ResolutionContext> auto const& context,
        tag_t tag
    )
        : resolver_ { [context, tag] {
            return context
                .container
                .template resolve<Type>(std::tuple { TagInput {
                    tag
                }});
        }}
    {}

public:
    ReturnType resolve() const
    {
        return this->resolver_();
    }

private:
    std::function<ReturnType()> resolver_;
};

}

#endif // !CAPYDI_DYNAMIC_RESOLVER_HPP_

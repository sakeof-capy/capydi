#ifndef TAG_CONFIG_HPP_
#define TAG_CONFIG_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/configs/inputs/TagInput.hpp"
#include "capydi/configs/inputs/NoInput.hpp"
#include "capydi/Error.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/primitives/referencing/RuntimeRef.hpp>
#include <tuple>
#include <expected>
#include <optional>
#include <algorithm>

namespace capy::di
{

namespace implementation_details_
{

template<CreationalConfig Decoratee>
class Tag
    : public DecoratableConfig<
        Tag<Decoratee>
    >
{
public:
    constexpr explicit Tag(tag_t tag, Decoratee&& decoratee)
        : tag_ { tag }
        , decoratee_ { std::move(decoratee) }
    {}

public:
    using CentralType = central_type_t<Decoratee>;
    using /* meta::Pack<meta::Pack<?>> */ ResolutionKeysPack = resolution_keys_pack_t<Decoratee>;
    using DependenciesPack = dependencies_pack_t<Decoratee>;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;

public:
    template<typename Key>
    std::expected<meta::RuntimeRef<Key>, Error> do_resolve(
        meta::Pack<Key>&& keys, 
        auto& dependencies,
        const auto& input
    ) const
    {
        std::optional<TagInput> tag_input = input.template retrieve_override<TagInput>();

        if (!tag_input.has_value()) [[unlikely]]
        {
            return std::unexpected { Error::TAG_CONFIG_EXPECTED }; 
        }

        const tag_t input_tag = tag_input.value().tag.value();

        if (this->tag_ != input_tag) [[unlikely]]
        {
            return std::unexpected { Error::TAG_MISMATCH };
        }

        return decoratee_.do_resolve(
            keys, 
            dependencies, 
            input
        );
    }

    template<std::size_t DependencyIndex>
    meta::wrapped_with<std::optional> auto get_dependencies_input() const
    {
        return this->decoratee_
            .template get_dependencies_input<DependencyIndex>();    
    }

private:
    tag_t tag_;
    Decoratee decoratee_;
};

}

struct Tag
{
    template<CreationalConfig Decoratee>
    static constexpr implementation_details_::Tag<Decoratee> decorate(
        Decoratee&& decoratee, 
        tag_t tag
    ) {
        return implementation_details_::Tag<Decoratee> {
            tag,
            std::forward<Decoratee>(decoratee)
        };
    }
};

}

#endif // !TAG_CONFIG_HPP_

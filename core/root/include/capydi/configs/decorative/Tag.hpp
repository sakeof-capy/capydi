#ifndef TAG_CONFIG_HPP_
#define TAG_CONFIG_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/configs/concepts/ChainableConfig.hpp"
#include "capydi/configs/decorative/DecoratableConfig.hpp"
#include "capydi/configs/inputs/TagInput.hpp"
#include "capydi/configs/inputs/NoInput.hpp"
#include "capydi/Error.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/primitives/referencing/RuntimeRef.hpp>
#include <capymeta/algorithms/pack/Contains.hpp>
#include <tuple>
#include <expected>
#include <optional>
#include <algorithm>

namespace capy::di
{

namespace implementation_details_
{

template<
    CreationalConfig Decoratee,
    typename TagComparator
>
class TagFilter
    : public DecoratableConfig<
        TagFilter<Decoratee, TagComparator>
    >
{
public:
    constexpr explicit TagFilter(TagComparator&& comparator, Decoratee&& decoratee)
        : decoratee_ { std::move(decoratee) }
        , comparator_ { std::move(comparator) }
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
        meta::wrapped_with<ResolutionContext> auto& context,
        const auto& input
    ) const
    {
        std::optional<TagInput> tag_input = input.template retrieve_override<TagInput>();

        if (!tag_input.has_value()) [[unlikely]]
        {
            return std::unexpected { Error::TAG_CONFIG_EXPECTED }; 
        }

        const tag_t input_tag = tag_input.value().tag.value();

        if (!this->comparator_(input_tag)) [[unlikely]]
        {
            return std::unexpected { Error::TAG_MISMATCH };
        }

        return decoratee_.do_resolve(
            keys, 
            dependencies, 
            context,
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
    Decoratee decoratee_;
    TagComparator comparator_;
};

template<
    ChainableConfig Decoratee, 
    typename TagComparator
>
class TagFilterChainable
    : public DecoratableConfig<
        TagFilterChainable<Decoratee, TagComparator>
    >
{
public:
    constexpr explicit TagFilterChainable(
        TagComparator&& comparator, 
        Decoratee&& decoratee
    )
        : decoratee_ { std::move(decoratee) }
        , comparator_ { std::move(comparator) }
    {}

public:
    using RelatedEntity = get_related_entity_t<Decoratee>;
    using RelatedKeysPack = get_related_keys_pack_t<Decoratee>;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CHAINABLE;

public:
    Resolution<RelatedEntity, Error> auto
        pipe(
            meta::Reference<RelatedEntity> auto decoratee,
            meta::wrapped_with<ResolutionContext> auto& context
        ) const
    {
        using InputsPack = meta::rebind_t<
            decltype(context.input),
            std::tuple, 
            meta::Pack 
        >;
        using ResultType = decltype(this->decoratee_.pipe(decoratee, context));

        if constexpr (meta::pack_contains_t<InputsPack, TagInput>)
        {
            const TagInput& tag_input = std::get<TagInput>(context.input);

            if (!this->comparator_(tag_input.tag.value()))
            {
                return ResultType {
                    decoratee
                };
            }

            return this->decoratee_.pipe(decoratee, context);
        }
        else
        {
            return ResultType {
                decoratee
            };
        }
    }


private:
    Decoratee decoratee_;
    TagComparator comparator_;
};

}

struct Tag
{
    template<CreationalConfig Decoratee>
    static constexpr auto decorate(
        Decoratee&& decoratee, 
        tag_t tag
    ) {
        return implementation_details_::TagFilter {
            [tag](const tag_t& inner_tag) { return inner_tag == tag; },
            std::forward<Decoratee>(decoratee)
        };
    }

    template<ChainableConfig Decoratee>
    static constexpr auto decorate_chainable(
        Decoratee&& decoratee, 
        tag_t tag
    ) {
        return implementation_details_::TagFilterChainable {
            [tag](const tag_t& inner_tag) { return inner_tag == tag; },
            std::forward<Decoratee>(decoratee)
        };
    }
};

struct TagFilter
{
    template<
        CreationalConfig Decoratee,
        typename TagComparator
    >
    static constexpr implementation_details_::TagFilter<Decoratee, TagComparator> 
        decorate(
            Decoratee&& decoratee, 
            TagComparator&& tag_comparator
        ) 
    {
        return implementation_details_::TagFilter {
            std::move(tag_comparator),
            std::forward<Decoratee>(decoratee)
        };
    }

    template<
        ChainableConfig Decoratee, 
        typename TagComparator
    >
    static constexpr implementation_details_::TagFilterChainable<Decoratee, TagComparator> 
        decorate_chainable(
            Decoratee&& decoratee, 
            TagComparator&& tag_comparator
        ) 
    {
        return implementation_details_::TagFilterChainable {
            std::move(tag_comparator),
            std::forward<Decoratee>(decoratee)
        };
    }
};

}

#endif // !TAG_CONFIG_HPP_

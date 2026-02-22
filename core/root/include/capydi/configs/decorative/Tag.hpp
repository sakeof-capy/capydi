#ifndef TAG_CONFIG_HPP_
#define TAG_CONFIG_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/configs/inputs/TagInput.hpp"
#include "capydi/configs/inputs/NoInput.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/primitives/referencing/RuntimeRef.hpp>
#include <tuple>

namespace capy::di
{

template<CreationalConfig Decoratee>
class Tag
{
public:
    constexpr explicit Tag(std::size_t tag, Decoratee&& decoratee)
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

        const std::size_t input_tag = tag_input.value().tag.value();

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

    // template<typename Key>
    // std::expected<meta::RuntimeRef<Key>, Error> do_resolve(
    //     meta::Pack<Key>&& keys, 
    //     auto& dependencies,
    //     const auto& input
    // ) const
    // {
    //     return std::unexpected { Error::UNRECOGNIZED_CONFIG_INPUT };
    // }

    template<std::size_t DependencyIndex>
    meta::wrapped_with<std::optional> auto get_dependencies_input() const
    {
        return this->decoratee_
            .template get_dependencies_input<DependencyIndex>();    
    }

private:
    std::size_t tag_;
    Decoratee decoratee_;
};

}

#endif // !TAG_CONFIG_HPP_

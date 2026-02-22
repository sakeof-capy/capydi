#ifndef CAPYDI_DEPENDENCY_TAGS_HPP_
#define CAPYDI_DEPENDENCY_TAGS_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/configs/inputs/TagInput.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/primitives/referencing/RuntimeRef.hpp>
#include <initializer_list>

#include <iostream>

namespace capy::di
{

template<CreationalConfig Decoratee>
class DependencyTags
{
public:
    using DependencyTagPair = std::pair<std::size_t, std::size_t>;

public:
    constexpr explicit DependencyTags(
        std::initializer_list<DependencyTagPair> dependency_tags, 
        Decoratee&& decoratee
    )
        : dependency_tags_ { dependency_tags }
        , decoratee_ { std::move(decoratee) }
    {}

public:
    using CentralType = central_type_t<Decoratee>;
    using /* meta::Pack<meta::Pack<?>> */ ResolutionKeysPack = resolution_keys_pack_t<Decoratee>;
    using DependenciesPack = dependencies_pack_t<Decoratee>;

public:
    static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;

public:
    template<typename... Args>
    auto do_resolve(Args&&... args) const
    {
        return decoratee_.do_resolve(std::forward<Args>(args)...);
    }

    template<std::size_t DependencyIndex>
    meta::wrapped_with<std::optional> auto get_dependencies_input() const
    {
        using DecorateeDependenciesTuple 
            = typename decltype(this->decoratee_.template get_dependencies_input<DependencyIndex>())::value_type;
        using ResultType = decltype(std::tuple_cat(
            std::declval<DecorateeDependenciesTuple>(),
            std::declval<std::tuple<TagInput>>()
        ));

        const auto found_tag_pair_itor = std::find_if(
            this->dependency_tags_.cbegin(),
            this->dependency_tags_.cend(),
            [](const DependencyTagPair& tag_pair) {
                return tag_pair.first == DependencyIndex;
            }
        );

        if (found_tag_pair_itor == this->dependency_tags_.cend()) [[unlikely]]
        {
            std::cout << "#" << DependencyIndex << ": HERE0" << std::endl;
            return std::optional<ResultType> {
                std::nullopt
            };
        }

        const auto& [_, found_tag] = *found_tag_pair_itor;

        auto decoratee_dependencies_input 
            = this->decoratee_.template get_dependencies_input<DependencyIndex>();
        
        if (decoratee_dependencies_input.has_value()) [[likely]] 
        {
            return std::optional { std::tuple_cat(
                std::move(decoratee_dependencies_input).value(),
                std::tuple { TagInput {
                    found_tag
                }}
            )};
        }
        else 
        {
            return std::optional { std::tuple_cat(
                std::tuple{},
                std::tuple { TagInput {
                    found_tag
                }}
            )}; 
        }
    }

private:
    std::vector<DependencyTagPair> dependency_tags_;
    Decoratee decoratee_;
};

}

#endif // !CAPYDI_DEPENDENCY_TAGS_HPP_

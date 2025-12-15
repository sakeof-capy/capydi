#ifndef DEPENDENCY_INFO_HPP_
#define DEPENDENCY_INFO_HPP_

#include "configs/concepts/CreationalConfig.hpp"
#include "utilities/pack/FunctionTraits.hpp"
#include "utilities/pack/Filter.hpp"
#include "utilities/pack/PackMap.hpp"
#include "utilities/pack/PackContains.hpp"
#include "utilities/IndexSequenceAsPack.hpp"
#include "utilities/MetaFunctor.hpp"

namespace capy::di
{


namespace hidden__
{   
    template<CreationalConfig Config>
    struct DependenciesPackOf;
}

template<CreationalConfig... Configs>
struct DependenciesInfo
{
private:
    using ConfigsPack = Pack<Configs...>;

    template<typename Key>
    struct IsConfigRelatedToTheKey
    {
        template<typename Config>
        struct Predicate
        {
        private:
            using Keys = resolution_keys_pack_t<Config>;

        public:
            static constexpr bool value = pack_contains_t<Keys, Key>;
        };
    };

    template<typename Key>
    using find_matching_config_t = unpack_or_none_t<
        filter_t<
            ConfigsPack, 
            IsConfigRelatedToTheKey<Key>::template Predicate
        >
    >;

    template<typename Key>
    using dependencies_pack_of_t = std::conditional_t<
        is_none_v<find_matching_config_t<Key>>,
        None,
        hidden__::DependenciesPackOf<find_matching_config_t<Key>>
    >;

    template<typename RelatedConfig, typename DependenciesPack>
    struct DependencyKeysPackOf
    {
        using OverridenKeys 
            = dependency_resolution_keys_overrides_pack_t<RelatedConfig>;

        template<std::size_t Index>
        using OverrideFinder = UnaryPredicate<
            []<
                std::size_t OverridenIndex, 
                typename OverridenKey
            >
            (
                Unit<Pack<
                    ValueUnit<OverridenIndex>, 
                    OverridenKey
                >>&&
            ) {
                return Index == OverridenIndex;
            }
        >;

        using type = pack_map_t<
            pack_enumerate_t<DependenciesPack>,
            MetaUnary<
                []<
                    std::size_t Index, 
                    typename Dependency
                >(Unit<Pack<ValueUnit<Index>, Dependency>>&&) {
                    using Override = filter_t<
                        OverridenKeys,
                        OverrideFinder<Index>::template F
                    >;

                    if constexpr (is_empty_pack_v<Override>) 
                    {
                        return Pack<
                            Dependency, 
                            default_key_t<RelatedConfig>
                        >{};
                    }
                    else 
                    {
                        using OverridenKey = pack_element_t<
                            unpack_t<Override>,
                            1
                        >;
                        
                        return Pack<
                            Dependency, 
                            OverridenKey
                        >{};
                    } 
                }
            >::template F
        >;
    };

public:

    template<typename Key>
    using dependency_keys_pack_of_t = 
        std::conditional_t<
            is_none_v<find_matching_config_t<Key>>,
            None,
            typename DependencyKeysPackOf<
                find_matching_config_t<Key>,
                dependencies_pack_of_t<Key>
            >::type
        >;
};

namespace hidden__
{
    template<typename Config>
    concept CreatableViaStaticMethod =
        CreationalConfig<Config> && 
        create_static_method_exists_and_is_unique_v<
            central_type_t<Config>
        >;

    template<typename Config>
    concept CreatableViaDependenciesPack = CreationalConfig<Config> && requires() {
        typename Config::DependenciesPack;
    };

    template<CreatableViaDependenciesPack Config>
    struct DependenciesPackOf<Config>
    {
        using type = typename Config::DependenciesPack;
    };

    template<CreatableViaStaticMethod Config>
    struct DependenciesPackOf<Config>
    {
        using type = args_pack_t<decltype(central_type_t<Config>::create)>;
    };
}

}

#endif // !DEPENDENCY_INFO_HPP_

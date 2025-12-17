#ifndef CONFIG_EXTRACTOR_HPP_
#define CONFIG_EXTRACTOR_HPP_

#include "capymeta/pack/Pack.hpp"
#include "capymeta/Rebind.hpp"
#include "capymeta/pack/FilterMap.hpp"
#include "capymeta/pack/Maybe.hpp"
#include "capymeta/pack/PackZip.hpp"

#include <utility>

namespace capy::di
{

/// @cond HIDDEN

namespace hidden__
{
    template<auto... Values>
    struct NormalizedIntegerSequence {};

    template<typename StdIndexSequence>
    struct Normalize;

    template<auto... Values>
    struct Normalize<
        std::integer_sequence<
            std::size_t, 
            Values...
        >
    >
    {
        using type = NormalizedIntegerSequence<Values...>;
    };

    template<typename StdIndexSequence>
    using normalize_t = typename Normalize<StdIndexSequence>::type;

    template<
        template<typename> typename Predicate
    >
    struct ConfigToIndexMapper
    {
        template<typename T>
        struct Mapper;

        template<typename Config, std::size_t Index>
        struct Mapper<meta::Pack<Config, meta::ValueUnit<Index>>>
        {
            using type = std::conditional_t<
                Predicate<Config>::value,
                meta::ValueUnit<Index>,
                meta::None
            >;
        };
    };

    template<
        template<typename> typename Predicate,
        typename... Configs
    >
    struct ConfigsToIndexSequence
    {
        using ConfigsPack = meta::Pack<Configs...>;

        using ConfigsIndexSequence = std::index_sequence_for<Configs...>;
        using NormalizedConfigsIndexSequence = normalize_t<ConfigsIndexSequence>;
        using ConfigsIndicesPack = meta::rebind_valued_as_typed_t<
            NormalizedConfigsIndexSequence,
            NormalizedIntegerSequence,
            meta::Pack
        >;
        using ConfigsToIndicesPack = meta::pack_zip_t<ConfigsPack, ConfigsIndicesPack>;
        
        using type = meta::filter_map_t<
            ConfigsToIndicesPack,
            ConfigToIndexMapper<Predicate>::template Mapper
        >;
    };
    
    template<
        template<typename> typename Predicate,
        typename... Configs
    >
    using configs_to_index_sequence_pack_t = typename ConfigsToIndexSequence<
        Predicate, 
        Configs...
    >::type;


    template<std::size_t... Idx, typename... Configs>
    [[nodiscard]] constexpr auto
        extract_config_tuple(
            meta::Pack<meta::ValueUnit<Idx>...>&&, 
            Configs&... configs
        )
    {
        std::tuple<Configs&...> configs_tuple = { configs... };
        return std::tuple { std::move(std::get<Idx>(configs_tuple))... };
    }
}

//// @endcond

template<
    template<typename> typename Predicate, 
    typename... Configs
>
[[nodiscard]] constexpr auto /* std::tuple<FiltratedConfigs...> */
    filter_configs(
        meta::UnaryMetaFunction<Predicate>&& predicates,
        Configs&... configs
    ) noexcept
{
    using ConfigsIndexSequencePack = 
        hidden__::configs_to_index_sequence_pack_t<
            Predicate,
            Configs...
        >;

    return hidden__::extract_config_tuple(
        ConfigsIndexSequencePack{},
        configs...
    );
}

}

#endif // !CONFIG_EXTRACTOR_HPP_

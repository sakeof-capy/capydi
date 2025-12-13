#ifndef CONFIG_EXTRACTOR_HPP_
#define CONFIG_EXTRACTOR_HPP_

#include "capydi/utilities/pack/Pack.hpp"
#include "capydi/utilities/Rebind.hpp"
#include "capydi/utilities/pack/FilterMap.hpp"
#include "capydi/utilities/pack/None.hpp"
#include "capydi/utilities/pack/PackZip.hpp"

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
        struct Mapper<Pack<Config, ValueUnit<Index>>>
        {
            using type = std::conditional_t<
                Predicate<Config>::value,
                ValueUnit<Index>,
                None
            >;
        };
    };

    template<
        template<typename> typename Predicate,
        typename... Configs
    >
    struct ConfigsToIndexSequence
    {
        using ConfigsPack = Pack<Configs...>;

        using ConfigsIndexSequence = std::index_sequence_for<Configs...>;
        using NormalizedConfigsIndexSequence = normalize_t<ConfigsIndexSequence>;
        using ConfigsIndicesPack = rebind_valued_as_typed_t<
            NormalizedConfigsIndexSequence,
            NormalizedIntegerSequence,
            Pack
        >;
        using ConfigsToIndicesPack = pack_zip_t<ConfigsPack, ConfigsIndicesPack>;
        
        using type = filter_map_t<
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
            Pack<ValueUnit<Idx>...>&&, 
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
        UnaryMetaFunction<Predicate>&& predicates,
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

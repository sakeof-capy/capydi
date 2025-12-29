#ifndef CAPYDI_DEPENDENCIESINFO_HPP_
#define CAPYDI_DEPENDENCIESINFO_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"

#include <capymeta/algorithms/pack/legacy/FunctionTraits.hpp>
#include <capymeta/type_structures/Maybe.hpp>
#include <capymeta/algorithms/pack/Find.hpp>
#include <capymeta/algorithms/pack/Map.hpp>
#include <capymeta/algorithms/pack/Contains.hpp>
#include <capymeta/primitives/Functor.hpp>
#include <capymeta/primitives/Template.hpp>
#include <capymeta/primitives/Overload.hpp>
#include <utility>

namespace capy::di
{

template<typename Config>
concept created_via_create_method = meta::create_static_method_exists_and_is_unique_v<
    central_type_t<Config>
>;

template<typename Entity>
using default_key_t = meta::Pack<Entity>;

template<CreationalConfig... Configs>
class DependenciesInfo
{
private:
    using ConfigsPack = meta::Pack<Configs...>;

    template<typename Key>
    using find_related_config_mt = meta::pack_find_t<
        ConfigsPack,
        meta::functor_fv<[]<class Config>(meta::Pack<Config>) {
            using ResolutionKeysPack = resolution_keys_pack_t<Config>;
            return meta::pack_contains_v<ResolutionKeysPack, Key>;
        }, meta::MetaArity::N1>
    >;

private:
    using StaticCreateMethodRetriever = meta::functor_ft<
        meta::Overload {
            []<created_via_create_method Config>(meta::Pack<Config>) {
                using CentralType = central_type_t<Config>;
                using DependencyReferencesPack
                    = meta::args_pack_t<decltype(CentralType::create)>;

                using DependenciesPack = meta::pack_map_t<
                    DependencyReferencesPack,
                    meta::functor_ft<[]<class T>(meta::Pack<T>) -> std::remove_reference_t<T> {
                        std::unreachable();
                    }, meta::MetaArity::N1>
                >;

                return meta::Some<DependenciesPack>{};
            },
            []<class Config>(meta::Pack<Config>) {
                return meta::None{};
            },
        },
        meta::MetaArity::N1
    >;

    using /* Pack<(config) -> ?> */ DependenciesRetrieversPack = meta::Pack<
        StaticCreateMethodRetriever
    >;

    template<CreationalConfig Config>
    using /* Maybe<(config) -> ?>*/ find_dependencies_retriever_mft = meta::pack_find_t<
        DependenciesRetrieversPack,
        meta::functor_fv<[]<class Retriever>(meta::Pack<Retriever>) {
            return meta::call_t<Retriever, Config>::HAS_VALUE;
        }, meta::MetaArity::N1>
    >;

    template<CreationalConfig Config>
    using /* Pack<?> */ retrieve_dependencies_of
        = find_dependencies_retriever_mft<Config>
        ::template Map<meta::functor_ft<
            []<class Retriever>(meta::Pack<Retriever>) {
                return meta::call_t<Retriever, Config>{};
            },
            meta::MetaArity::N1>
        >;

public:
    template<typename Key>
    using dependency_keys_pack_of_mt
        = find_related_config_mt<Key>
        ::template AndThen<meta::template_ft<
            retrieve_dependencies_of,
            meta::MetaArity::N1
        >>
        ::template Map<meta::functor_ft<
            []<class DependenciesPack>(meta::Pack<DependenciesPack>) {
                return meta::pack_map_t<
                    DependenciesPack,
                    meta::functor_ft<[]<class Dependency> {
                        return default_key_t<Dependency>{};
                    }, meta::MetaArity::N1>
                >{};
            },
        meta::MetaArity::N1>>;
};

}

#endif //CAPYDI_DEPENDENCIESINFO_HPP_

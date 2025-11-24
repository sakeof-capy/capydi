#ifndef CREATIONAL_CONFIG_HPP_
#define CREATIONAL_CONFIG_HPP_

#include "utilities/referencing/Reference.hpp"

namespace capy::di
{

namespace hidden__
{
    struct DummyDependency1 {};
    struct DummyDependency2 {};
    
    template<typename Config, typename CentralType>
    concept CreationalConfigWithCentralType = requires(Config config) {
        { 
            config.do_resolve(
                Pack<CentralType>{}, 
                std::tuple<DummyDependency1, DummyDependency2>{}
            ) 
        } -> Reference<CentralType>;
    };
}

template<typename Config>
concept CreationalConfig = requires() {
    typename Config::CentralType;
    typename Config::ResolutionKeysPack;
};
// && (
//     hidden__::CreationalConfigWithCentralType<
//         Config,
//         typename Config::CentralType
//     > || 
//     hidden__::CreationalConfigWithCentralType<
//         Config,
//         const typename Config::CentralType
//     >
// ); 

template<CreationalConfig Config>
using central_type_t = typename Config::CentralType;

template<CreationalConfig Config>
using resolution_keys_pack_t = typename Config::ResolutionKeysPack;

}

#endif // !CREATIONAL_CONFIG_HPP_

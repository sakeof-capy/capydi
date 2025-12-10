#ifndef CONTAINER_TRY_HPP_
#define CONTAINER_TRY_HPP_

#include "configs/ConfigType.hpp"
#include "configs/concepts/ChainableConfig.hpp"
#include "configs/concepts/CreationalConfig.hpp"
#include "configs/concepts/DiConfig.hpp"
#include "dispatchers/CreationalConfigDispatcher.hpp"
#include "dispatchers/ChainableConfigDispatcher.hpp"
#include "utilities/pack/Pack.hpp"
#include "utilities/pack/Filter.hpp"
#include "utilities/Rebind.hpp"
#include "configs/ConfigClassifier.hpp"

#include <tuple>
#include <utility>

/**
 * @dot
 * digraph G {
 *     A -> B;
 *     // no Helper here
 * }
 * @enddot
 */


namespace capy::di
{

template<DiConfig... Configs>
class DI
{
private:
    using ConfigsPack = Pack<Configs...>;
    using CreationalConfigsPack = filter_t<ConfigsPack, IsCreationalConfig>;
    using ChainableConfigsPack = filter_t<ConfigsPack, IsChainableConfig>;
    
    using CreationalDispatcherType = rebind_pack_t<
        CreationalConfigsPack, 
        CreationalConfigDispatcher
    >; 
    
    using ChainableDispatcherType = rebind_pack_t<
        ChainableConfigsPack, 
        ChainableConfigDispatcher
    >;

public:
    explicit constexpr DI(Configs&&... configs)
        : creational_dispatcher_ { make_dispatcher(
            Unit<CreationalDispatcherType>{},
            filter_configs(
                UnaryMetaFunction<IsCreationalConfig>{},
                configs...
            )
        )}
        , chainable_dispatcher_ { make_dispatcher(
            Unit<ChainableDispatcherType>{},
            filter_configs(
                UnaryMetaFunction<IsChainableConfig>{},
                configs...
            )
        )}
    {}

public:
    template<Creatable Type>
    [[nodiscard]] constexpr Resolution<Type, Error> auto resolve() const
    {
        /* TODO: implement dispatcher for retrieving key */
        using /* Pack<?> */ KeyPack = Pack<Type>;

        return this->creational_dispatcher_
            .template resolve<Type>()
            .and_then([this](Reference<Type> auto entity) {
                return this->chainable_dispatcher_
                    .template apply_configs_chain<KeyPack, Type>(entity);
            });
    }

private:
    template<typename Dispatcher, typename... Configs_> 
    [[nodiscard]] static constexpr Dispatcher 
        make_dispatcher(
            Unit<Dispatcher>&&, 
            std::tuple<Configs_...>&& configs_tuple
        )
    {
        return std::apply(
            [](auto&&... configs) {
                return Dispatcher(
                    std::forward<decltype(configs)>(configs)...
                );
            }, 
            std::move(configs_tuple)
        );
    }

private:
    CreationalDispatcherType creational_dispatcher_;
    ChainableDispatcherType chainable_dispatcher_;
};

}

#endif // !CONTAINER_TRY_HPP_

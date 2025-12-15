/**
 * @file Container.hpp
 * @brief Core dependency injection container for managing creational and chainable configurations.
 * 
 * The DI container is the heart of the Capydi framework. It orchestrates both creational
 * and chainable configuration dispatchers to resolve dependencies with compile-time safety
 * and runtime efficiency. The container separates dependencies into two categories:
 * 
 * - **Creational configs**: Instantiate and manage object creation
 * - **Chainable configs**: Decorate and transform objects through a pipeline (e.g., Decorators)
 * 
 * @section architecture Architecture
 * 
 * The DI container uses a two-dispatcher pattern:
 * 
 * @dot
 * digraph DI_Architecture {
 *     rankdir=LR;
 *     DI [shape=box, label="DI Container\n(Configs...)"];
 *     Creational [shape=box, label="CreationalConfigDispatcher"];
 *     Chainable [shape=box, label="ChainableConfigDispatcher"];
 *     Entity [shape=ellipse, label="Entity<T>"];
 *     
 *     DI -> Creational [label="filters\ncreational"];
 *     DI -> Chainable [label="filters\nchainable"];
 *     Creational -> Entity [label="creates"];
 *     Chainable -> Entity [label="decorates"];
 * }
 * @enddot
 * 
 * @section example Usage Example
 * 
 * @code
 * // Create a container with both creational and chainable configs
 * auto di = DI(
 *     CreationalConfig<MyService>{},
 *     Decorator<MyDecorator>{}
 * );
 * 
 * // Resolve a dependency with optional decoration
 * auto result = di.resolve<MyKey>();
 * @endcode
 */

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


namespace capy::di
{

/**
 * @class DI
 * @brief Primary dependency injection container managing configurations and resolution.
 * 
 * @tparam Configs Parameter pack of config types satisfying the DiConfig concept.
 *         These are automatically classified as either CreationalConfig or ChainableConfig.
 * 
 * The DI container stores two internal dispatchers that handle different aspects:
 * - **CreationalDispatcher**: Manages creation and lifetime of dependencies
 * - **ChainableDispatcher**: Applies decorators and transformations to created objects
 * 
 * All resolution happens with compile-time type safety via concepts and template specialization.
 * The container filters the input configs into two groups and rebinds them to their respective
 * dispatchers, creating a type-safe, zero-overhead abstraction.
 * 
 * @note The container is @c constexpr-compatible, enabling compile-time DI setup where possible.
 */

template<typename... Configs>
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
    template<typename Type, typename KeyPack = Pack<Type>>
    [[nodiscard]] constexpr Resolution<Type, Error> auto resolve() const
    {
        /* TODO: implement dispatcher for retrieving key */
        // using /* Pack<?> */ KeyPack = Pack<Type>;

        return this->creational_dispatcher_
            .template resolve<Type, KeyPack>()
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

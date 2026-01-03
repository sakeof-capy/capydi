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

#include "configs/concepts/ChainableConfig.hpp"
#include "configs/concepts/CreationalConfig.hpp"
#include "dispatchers/CreationalConfigDispatcher.hpp"
#include "dispatchers/ChainableConfigDispatcher.hpp"
#include "configs/ConfigClassifier.hpp"

#include <capymeta/primitives/Pack.hpp>
#include <capymeta/algorithms/pack/Filter.hpp>
#include <capymeta/primitives/Functor.hpp>
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
    using ConfigsPack = meta::Pack<Configs...>;
    using CreationalConfigsPack = meta::pack_filter_t<
        ConfigsPack,
        meta::template_fv<IsCreationalConfig, meta::MetaArity::N1>
    >;
    using ChainableConfigsPack = meta::pack_filter_t<
        ConfigsPack,
        meta::template_fv<IsChainableConfig, meta::MetaArity::N1>
    >;
    
    using CreationalDispatcherType = meta::rebind_pack_t<
        CreationalConfigsPack, 
        CreationalConfigDispatcher
    >; 
    
    using ChainableDispatcherType = meta::rebind_pack_t<
        ChainableConfigsPack, 
        ChainableConfigDispatcher
    >;

public:
    explicit constexpr DI(Configs&&... configs)
        : creational_dispatcher_ { make_dispatcher(
            meta::Unit<CreationalDispatcherType>{},
            filter_configs(
                meta::UnaryMetaFunction<IsCreationalConfig>{},
                configs...
            )
        )}
        , chainable_dispatcher_ { make_dispatcher(
            meta::Unit<ChainableDispatcherType>{},
            filter_configs(
                meta::UnaryMetaFunction<IsChainableConfig>{},
                configs...
            )
        )}
    {}

public: 
    template<Creatable Type, typename KeyPack = meta::Pack<Type>>
    [[nodiscard]] constexpr Resolution<Type, Error> auto resolve() const
    {
        /* TODO: implement dispatcher for retrieving key */
        // using /* meta::Pack<?> */ KeyPack = meta::Pack<Type>;

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
            meta::Unit<Dispatcher>&&, 
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

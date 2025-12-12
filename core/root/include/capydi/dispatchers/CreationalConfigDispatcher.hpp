/**
 * @file CreationalConfigDispatcher.hpp
 * @brief Dispatcher responsible for creating and resolving creational dependencies.
 * 
 * The CreationalConfigDispatcher handles the first phase of dependency resolution:
 * instantiation and creation of objects. It routes resolution requests to the
 * appropriate CreationalConfig based on the requested type.
 * 
 * @see CreationalConfig
 * @see DI
 */

#ifndef CREATIONAL_CONFIG_DISPATCHER_HPP_
#define CREATIONAL_CONFIG_DISPATCHER_HPP_

#include "configs/concepts/CreationalConfig.hpp"
#include "utilities/pack/Pack.hpp"
#include "utilities/pack/PackAlgorithm.hpp"
#include "utilities/pack/FunctionTraits.hpp"
#include "utilities/referencing/Reference.hpp"
#include "utilities/referencing/RuntimeRef.hpp"
#include "Resolution.hpp"
#include "Error.hpp"

#include <expected>
#include <tuple>

namespace capy::di
{

/// @cond IMPLEMENTATION

template<typename T>
concept Creatable = create_static_method_exists_and_is_unique_v<T>;

/// @endcond

/**
 * @class CreationalConfigDispatcher
 * @brief Routes and executes creational resolution requests to appropriate configs.
 * 
 * @tparam Configs Variable number of CreationalConfig types.
 * 
 * This dispatcher maintains a collection of creational configurations and provides
 * a @c resolve<Type>() interface to create instances. It handles:
 * - Type-based routing to the correct config
 * - Dependency resolution recursion
 * - Error handling via @c std::expected
 * 
 * @internal This is an internal component of the DI framework. Users interact with
 *           it indirectly through the DI container.
 * 
 * @see CreationalConfig
 * @see DI
 */
template<CreationalConfig... Configs>
class CreationalConfigDispatcher : Configs...
{
private:
    using Configs::do_resolve...;

    template<typename T>
    using dependencies_of_t = args_pack_t<decltype(T::create)>;

public:
    constexpr explicit CreationalConfigDispatcher(
        Configs&&... configs
    )
        : Configs { std::forward<Configs>(configs) }...
    {}

public:
    /**
     * @brief Resolve (create) an instance of the specified type.
     * 
     * @tparam Type The type to create. Must satisfy Creatable.
     * 
     * @returns @c std::expected<RuntimeRef<Type>, Error> containing either:
     *          - A reference to the created instance on success
     *          - An error code if creation failed or dependencies couldn't be resolved
     */
    template<Creatable Type>
    constexpr Resolution<Type, Error> auto resolve() const
    {
        using /* Pack<?> */ KeyPack = Pack<Type>;
        using /* Pack<?> */ Dependencies = dependencies_of_t<Type>;

        #define RESOLUTION_CALL \
            this->do_resolve(KeyPack{}, resolved_dependencies)

        return 
            valued_pack_for(
                Dependencies{},
                [this]<typename T>(Unit<T&>) -> Resolution<T, Error> auto {
                    return this->resolve<T>();
                }
            )
            .transform_error([](const auto&) {
                return Error::DEPENDENCY_CANNOT_BE_RESOLVED;
            })
            .and_then([this](auto&& resolved_dependencies) {
                if constexpr (requires { RESOLUTION_CALL; }) 
                {
                    using ReturnValue = decltype(RESOLUTION_CALL);
                    return std::expected<ReturnValue, Error> { RESOLUTION_CALL };
                } 
                else 
                {
                    return std::expected<RuntimeRef<Type>, Error> {
                        std::unexpected { Error::CANNOT_BE_RESOLVED }
                    };
                }
            });

        #undef RESOLUTION_CALL
    }
};

}

#endif // !CREATIONAL_CONFIG_DISPATCHER_HPP_

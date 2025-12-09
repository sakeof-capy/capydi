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

template<typename T>
concept Creatable = create_static_method_exists_and_is_unique_v<T>;

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
    template<Creatable Type>
    constexpr Resolution<Type, Error> auto resolve() const
    {
        using /* Pack<?> */ KeyPack = Pack<Type>;
        using /* Pack<?> */ Dependencies = dependencies_of_t<Type>;

        auto error = [](Error error_code) {
            return std::expected<RuntimeRef<Type>, Error> {  // TODO: replace RuntimeRef
                std::unexpected { error_code } 
            };
        };

        auto resolved_dependencies_result = valued_pack_for(
            Dependencies{},

            // TODO: provide a separate DependenciesResolver entity
            //       to handle different types of arguments (e.g. std::shared_ptr)
            [this]<typename T>(Unit<T&>) /* -> std::expected<Reference<Type>, Error> */ {
                return this->resolve<std::remove_reference_t<T>>();
            }
        ); 

        if (!resolved_dependencies_result.has_value()) [[unlikely]]
        {
            return error(Error::DEPENDENCY_CANNOT_BE_RESOLVED);
        }

        auto resolved_dependencies = resolved_dependencies_result.value();

        #define RESOLUTION_CALL \
            this->do_resolve(KeyPack{}, resolved_dependencies)
        
        if constexpr (requires { RESOLUTION_CALL; }) 
        {   
            using ReturnValue = decltype(RESOLUTION_CALL);
            return std::expected<ReturnValue, Error> { RESOLUTION_CALL };
        } 
        else 
        {
            return error(Error::CANNOT_BE_RESOLVED);
        }

        #undef RESOLUTION_CALL
    }
};

}

#endif // !CREATIONAL_CONFIG_DISPATCHER_HPP_

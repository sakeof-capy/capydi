#ifndef CONTAINER_HPP_
#define CONTAINER_HPP_

#include "utilities/pack/Pack.hpp"
#include "utilities/pack/PackAlgorithm.hpp"
#include "utilities/pack/FunctionTraits.hpp"
#include "utilities/referencing/Reference.hpp"

#include "utilities/referencing/RuntimeRef.hpp"

#include <expected>
#include <tuple>

namespace capy::di
{

enum class DiError
{
    CANNOT_BE_RESOLVED,
    DEPENDENCY_CANNOT_BE_RESOLVED,
};

template<typename T, typename Dependency>
concept ResolutionResult = true; /* std::expected<Reference<Type>, DiError> */

template<typename T>
concept Creatable = create_static_method_exists_and_is_unique_v<T>;

template<typename... Configs>
class DI: private Configs...
{
private:
    using Configs::do_resolve...;

    template<typename T>
    using dependencies_of_t = args_pack_t<decltype(T::create)>;

    // TODO: check if Type::create function returns value of Type
    
public:
    constexpr explicit DI(Configs&&... configs) 
        : Configs(std::move(configs))...
    {}

public:
    template<Creatable Type, typename... Keys>
    constexpr ResolutionResult<Type> auto resolve() const
    {
        using /* Pack<?> */ KeyPack = Pack<Type, Keys...>;
        using /* Pack<?> */ Dependencies = dependencies_of_t<Type>;

        auto error = [](DiError error_code) {
            return std::expected<RuntimeRef<Type>, DiError> { 
                std::unexpected { error_code } 
            };
        };

        auto resolved_dependencies_result = valued_pack_for(
            Dependencies{},

            // TODO: provide a separate DependenciesResolver entity
            //       to handle different types of arguments (e.g. std::shared_ptr)
            [this]<typename T>(Unit<T&>) /* -> std::expected<Reference<Type>, DiError> */ {
                return this->resolve<std::remove_reference_t<T>>();
            }
        ); 

        if (!resolved_dependencies_result.has_value()) [[unlikely]]
        {
            return error(DiError::DEPENDENCY_CANNOT_BE_RESOLVED);
        }

        auto resolved_dependencies = resolved_dependencies_result.value();

        #define RESOLUTION_CALL \
            this->do_resolve(KeyPack{}, resolved_dependencies)
        
        if constexpr (requires { RESOLUTION_CALL; }) 
        {   
            using ReturnValue = decltype(RESOLUTION_CALL);
            return std::expected<ReturnValue, DiError> { RESOLUTION_CALL };
        } 
        else 
        {
            return error(DiError::CANNOT_BE_RESOLVED);
        }
    }

    // template<typename Type, typename... Keys>
    // constexpr auto resolve() const
    // {
    // }
};

}

#endif // !CONTAINER_HPP_

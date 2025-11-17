#ifndef CONTAINER_HPP_
#define CONTAINER_HPP_

#include "utilities/Pack.hpp"
#include "utilities/None.hpp"
#include "utilities/PackAlgorithm.hpp"
#include "utilities/FunctionTraits.hpp"

#include <tuple>

namespace capy::di
{

template<typename T>
concept Creatable = create_static_method_exists_and_is_unique_v<T>;

template<typename... Configs>
class DI: private Configs...
{
private:
    using Configs::do_resolve...;

    template<typename T>
    using dependencies_of_t = ParamPackOf<decltype(T::create)>;
    
public:
    constexpr explicit DI(Configs&&... configs) 
        : Configs(std::move(configs))...
    {}

public:
    template<Creatable Type>
    constexpr Type& resolve() const
    {
        using /* Pack<?> */ KeyPack = Pack<Type>;
        using /* Pack<?> */ Dependencies = dependencies_of_t<Type>;

        rebind_pack_t<Dependencies, std::tuple> resolved_dependencies = valued_pack_for(
            Dependencies{},
            [this]<typename T>(Unit<T&>) -> T& {
                return this->resolve<std::remove_reference_t<T>>();
            }
        ); 

        return this->do_resolve(KeyPack{}, resolved_dependencies);
    }
};

}

#endif // !CONTAINER_HPP_

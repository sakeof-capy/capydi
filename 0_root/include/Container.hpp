#ifndef CONTAINER_HPP_
#define CONTAINER_HPP_

#include "utilities/Pack.hpp"
#include "utilities/None.hpp"
#include "utilities/PackAlgorithm.hpp"
#include "utilities/FunctionTraits.hpp"
#include "utilities/referencing/Reference.hpp"

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

    // TODO: check if Type::create function returns value of Type
    
public:
    constexpr explicit DI(Configs&&... configs) 
        : Configs(std::move(configs))...
    {}

public:
    template<Creatable Type>
    constexpr Reference<Type> auto resolve() const
    {
        using /* Pack<?> */ KeyPack = Pack<Type>;
        using /* Pack<?> */ Dependencies = dependencies_of_t<Type>;

        auto resolved_dependencies = valued_pack_for(
            Dependencies{},

            // TODO: provide a separate DependenciesResolver entity
            //       to handle different types of arguments (e.g. std::shared_ptr)
            [this]<typename T>(Unit<T&>) {
                return this->resolve<std::remove_reference_t<T>>();
            }
        ); 

        // using a = decltype(this->do_resolve(KeyPack{}, resolved_dependencies));
        // static_assert(std::is_same_v<a, int>, "asdpfkaspodfpas00");

        return this->do_resolve(KeyPack{}, resolved_dependencies);
    }
};

}

#endif // !CONTAINER_HPP_

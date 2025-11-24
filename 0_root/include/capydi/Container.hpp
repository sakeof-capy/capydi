#ifndef CONTAINER_HPP_
#define CONTAINER_HPP_

#include "utilities/pack/Pack.hpp"
#include "utilities/pack/PackAlgorithm.hpp"
#include "utilities/pack/FunctionTraits.hpp"
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
    using dependencies_of_t = args_pack_t<decltype(T::create)>;

    // TODO: check if Type::create function returns value of Type
    
public:
    constexpr explicit DI(Configs&&... configs) 
        : Configs(std::move(configs))...
    {}

public:
    template<Creatable Type, typename... Keys>
    constexpr Reference<Type> auto resolve() const
    {
        using /* Pack<?> */ KeyPack = Pack<Type, Keys...>;
        using /* Pack<?> */ Dependencies = dependencies_of_t<Type>;

        // TODO: provide a static assert to check if do_resolve 
        //       can exists

        auto resolved_dependencies = valued_pack_for(
            Dependencies{},

            // TODO: provide a separate DependenciesResolver entity
            //       to handle different types of arguments (e.g. std::shared_ptr)
            [this]<typename T>(Unit<T&>) {
                return this->resolve<std::remove_reference_t<T>>();
            }
        ); 

        return this->do_resolve(KeyPack{}, resolved_dependencies);
    }
};

}

#endif // !CONTAINER_HPP_

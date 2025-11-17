#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include <tuple>

namespace capy::di
{

template<typename Type>
struct Singleton
{
protected:
    template<typename... Dependencies>
    constexpr Type& do_resolve(Pack<Type> keys, std::tuple<Dependencies...>& dependencies) const
    {
        static Type instance = std::apply(Type::create, dependencies);
        return instance;
    }
};

}

#endif // !SINGLETON_HPP
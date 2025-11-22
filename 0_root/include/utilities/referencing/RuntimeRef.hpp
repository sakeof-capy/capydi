#ifndef RUNTIME_REF_HPP_
#define RUNTIME_REF_HPP_

#include <type_traits>

namespace capy::di
{

template<typename T>
class RuntimeRef 
{
private:
    static constexpr bool IS_CONST = std::is_const_v<T>;

public:
    constexpr /* implicit */ RuntimeRef(T& ref)
        : ref_ { ref }
    {}

public:
    constexpr operator T&() const noexcept requires (!IS_CONST) 
    {
        return ref_;
    }

    constexpr operator const T&() const noexcept requires (IS_CONST)
    {
        return ref_;
    }

private:
    T& ref_;
};

}

#endif // !RUNTIME_REF_HPP_

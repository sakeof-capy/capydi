#ifndef CONSTEXPR_REF_HPP_
#define CONSTEXPR_REF_HPP_

namespace capy::di
{

template<typename T, const T& Ref>
struct ConstexprRef 
{
public:
    constexpr operator const T&() const noexcept
    {
        return Ref;
    }
};

}

#endif // !CONSTEXPR_REF_HPP_

#ifndef ARITY_HPP_
#define ARITY_HPP_

#include <cstddef>
#include <optional>

namespace capy::meta
{

enum class MetaArity
{
    N1,
    N2,
    N3,
    N4,
    N5,
    N6,
    N7,
    N8,
    N9,
    N10,
    N11,
    N12,
    N13,
    N14,
    N15,
    N16,

    LINEAR_ARITIES_COUNT,
};

constexpr std::size_t MAX_LINEAR_META_ARITY = static_cast<std::size_t>(
    MetaArity::LINEAR_ARITIES_COUNT
);

template<typename... Args>
    requires (
        sizeof...(Args) > 0 && 
        sizeof...(Args) <= MAX_LINEAR_META_ARITY
    )
constexpr MetaArity meta_arity_from_args_v = static_cast<MetaArity>(sizeof...(Args) - 1);
    
}

#endif // !ARITY_HPP_

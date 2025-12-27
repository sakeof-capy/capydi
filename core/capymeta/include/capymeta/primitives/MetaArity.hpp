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

consteval std::optional<MetaArity>
    meta_arity_from_args_count(const std::size_t args_count)
{
    if (args_count < 1 || args_count > MAX_LINEAR_META_ARITY) {
        return std::nullopt;
    }

    return static_cast<MetaArity>(args_count - 1);
}

template<std::size_t ARGS_COUNT>
    requires (meta_arity_from_args_count(ARGS_COUNT).has_value())
constexpr MetaArity meta_arity_from_args_count_v
    = meta_arity_from_args_count(ARGS_COUNT).value();

template<typename... Args>
constexpr MetaArity meta_arity_from_args_v = meta_arity_from_args_count_v<sizeof...(Args)>;
    
}

#endif // !ARITY_HPP_

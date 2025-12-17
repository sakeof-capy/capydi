/**
 * @file Pack.hpp
 * @brief Compile-time type pack utilities and metaprogramming foundations.
 * 
 * This header provides the fundamental type manipulation utilities used throughout
 * Capydi for compile-time introspection, filtering, and rebinding. All operations
 * are performed at compile time with zero runtime overhead.
 * 
 * Key abstractions:
 * - @c Pack: A compile-time type list
 * - @c Unit: A wrapper for forwarding type information without instantiation
 * - @c ValueUnit: A wrapper for non-type template parameters (e.g., integers, bools)
 * - @c UnaryMetaFunction: A tag for template template parameters
 */

#ifndef PACK_HPP_
#define PACK_HPP_

#include <tuple>

namespace capy::meta
{

/**
 * @class Unit
 * @brief A zero-cost wrapper for forwarding compile-time type information.
 * 
 * @tparam UnitType The type being wrapped.
 * 
 * Unit is useful for passing type information through template metafunctions
 * without creating actual instances. It serves as a "type carrier" in template code.
 * 
 * @example
 * @code
 * template<typename Dispatcher>
 * void initialize(Unit<Dispatcher>&& u) {
 *     // Dispatcher type information is available at compile time,
 *     // but no instantiation has occurred
 * }
 * 
 * initialize(Unit<MyDispatcher>{});
 * @endcode
 */
template<typename UnitType>
struct Unit {};

/**
 * @class Pack
 * @brief A compile-time heterogeneous type list.
 * 
 * @tparam Types The types in the pack.
 * 
 * Pack is the primary abstraction for storing and manipulating groups of types
 * at compile time. It enables:
 * - Type-safe filtering and transformation
 * - Rebinding to different template containers
 * - Iteration and aggregation over heterogeneous types
 * 
 * @code
 * using MyPack = Pack<int, double, std::string>;
 * using Filtered = filter_t<MyPack, IsIntegral>;  // Pack<int>
 * @endcode
 */
template<typename... Types>
struct Pack {};

/**
 * @class ValueUnit
 * @brief A wrapper for non-type template parameter values.
 * 
 * @tparam Value An auto-typed non-type template parameter (integer, bool, etc.).
 * 
 * ValueUnit allows storing and manipulating NTTP values within type packs.
 * This is essential for working with std::index_sequence and similar constructs.
 * 
 * @code
 * using Seq = Pack<ValueUnit<0>, ValueUnit<1>, ValueUnit<2>>;
 * @endcode
 */
template<auto Value>
struct ValueUnit {};

/**
 * @class UnwrapValueUnit
 * @brief Type trait that extracts a value from a ValueUnit.
 * 
 * @tparam ValueUnit_ A ValueUnit instantiation.
 * 
 * Primary template is intentionally left unspecialized; specializations
 * extract the value from specific ValueUnit instances.
 */
template<typename ValueUnit_>
struct UnwrapValueUnit;

/// Specialization that extracts the stored value from ValueUnit<Value>
template<auto Value>
struct UnwrapValueUnit<ValueUnit<Value>>
{
    static constexpr auto value = Value;
};

/// Helper alias for easy extraction: @c unwrap_value_unit_v<ValueUnit<42>> evaluates to 42
template<typename ValueUnit_>
static constexpr auto unwrap_value_unit_v = UnwrapValueUnit<ValueUnit_>::value;

/**
 * @class UnaryMetaFunction
 * @brief A tag for template template parameters representing unary predicates.
 * 
 * @tparam Function A predicate template like @c IsIntegral<T> or @c IsCreationalConfig<T>.
 * 
 * This allows passing predicates as template arguments, enabling generic filtering
 * and selection algorithms in the pack utilities.
 */
template<
    template<typename> typename
> 
struct UnaryMetaFunction {};

/// The empty type pack; useful as a base case for recursive operations
using EmptyPack = Pack<>;

/**
 * @class RebindPack
 * @brief Type trait that repackages a Pack's elements into a different template.
 * 
 * @tparam Pack_ A Pack instantiation.
 * @tparam Template The target template to rebind into.
 * 
 * This enables conversion between heterogeneous containers without duplicating
 * type lists. For example, converting a @c Pack<int, double> into a tuple type
 * or a custom container.
 * 
 * @code
 * using MyPack = Pack<int, double, std::string>;
 * using MyTuple = rebind_pack_t<MyPack, std::tuple>;  // std::tuple<int, double, std::string>
 * @endcode
 */
template<typename Pack_, template<typename...> typename Template>
struct RebindPack;

/// Specialization: extract elements from Pack<Elements...> and apply Template
template<template<typename...> typename Template, typename... Elements>
struct RebindPack<Pack<Elements...>, Template>
{
    using type = Template<Elements...>;
};

/// Convenience alias for RebindPack
template<typename Pack_, template<typename...> typename Template>
using rebind_pack_t = typename RebindPack<Pack_, Template>::type;

}

#endif // !PACK_HPP_

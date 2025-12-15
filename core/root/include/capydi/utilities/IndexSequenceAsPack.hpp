#ifndef INDEX_SEQUENCE_AS_PACK_HPP_
#define INDEX_SEQUENCE_AS_PACK_HPP_

#include "capydi/utilities/Rebind.hpp"
#include "capydi/utilities/pack/PackZip.hpp"

#include <utility>

namespace capy::di
{

template<auto... Values>
struct NormalizedIntegerSequence {};

template<typename StdIndexSequence>
struct Normalize;

template<auto... Values>
struct Normalize<
    std::integer_sequence<
        std::size_t, 
        Values...
    >
>
{
    using type = NormalizedIntegerSequence<Values...>;
};

template<typename StdIndexSequence>
using normalize_t = typename Normalize<StdIndexSequence>::type;

template<typename StdIndexSequence>
struct IndexSequenceAsPack
{
private:
    using NormalizedConfigsIndexSequence = normalize_t<StdIndexSequence>;
    using ConfigsIndicesPack = rebind_valued_as_typed_t<
        NormalizedConfigsIndexSequence,
        NormalizedIntegerSequence,
        Pack
    >;

public:
    using type = ConfigsIndicesPack;
};

template<typename StdIndexSequence>
using index_sequence_as_pack_t 
    = typename IndexSequenceAsPack<StdIndexSequence>::type;

template<typename Pack_>
struct PackEnumerate;

template<typename... Elements>
struct PackEnumerate<Pack<Elements...>>
{
private:
    using StdSequence = std::index_sequence_for<Elements...>;
    using PackSequence = index_sequence_as_pack_t<StdSequence>;
    using EnumeratedPack = pack_zip_t<PackSequence, Pack<Elements...>>;

public:
    using type = EnumeratedPack;
};

template<typename Pack_>
using pack_enumerate_t = typename PackEnumerate<Pack_>::type;

}

#endif // !INDEX_SEQUENCE_AS_PACK_HPP_

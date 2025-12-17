#ifndef NONE_HPP_
#define NONE_HPP_

namespace capy::meta
{

namespace hidden__
{
    struct Nullopt{};
}

template<typename T>
struct Maybe;

using None = Maybe<hidden__::Nullopt>;

template<typename T>
    requires (!std::same_as<hidden__::Nullopt, T>)
using Some = Maybe<T>;

template<typename T>
struct Maybe
{
    static constexpr bool HAS_VALUE = true;

    using Value = T;

    template<
        template<typename> typename Mapper
    >
    using Map = Maybe<typename Mapper<T>::type>;

    template<
        template<typename> typename Predicate
    >
    using Filter = std::conditional_t<
        Predicate<T>::value,
        Some<T>,
        None
    >;

    template<
        template<typename> typename Mapper
    >
    using AndThen = typename Mapper<T>::type;

    template<typename OtherValue>
    using Or = Maybe<T>;
};

template<>
struct Maybe<hidden__::Nullopt>
{
    static constexpr bool HAS_VALUE = false;

    using Value = hidden__::Nullopt;

    template<
        template<typename> typename Mapper
    >
    using Map = None;

    template<
        template<typename> typename Predicate
    >
    using Filter = None;

    template<
        template<typename> typename Mapper
    >
    using AndThen = None;

    template<typename OtherValue>
    using Or = Maybe<OtherValue>;
};

}

#endif // NONE_HPP_

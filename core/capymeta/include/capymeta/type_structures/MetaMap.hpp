#include <capymeta/primitives/Pack.hpp>
#include <capymeta/algorithms/pack/Contains.hpp>
#include <capymeta/concepts/WrappedWIth.hpp>
#include <capymeta/primitives/referencing/RuntimeRef.hpp>
#include <capymeta/type_structures/Maybe.hpp>
#include <capymeta/type_structures/StaticMaybe.hpp>
#include <capymeta/primitives/Overload.hpp>

namespace capy::meta
{

template<typename Key, typename Value>
class KVPair
{
public:
    constexpr KVPair(Unit<Key>&&, Value&& value)
        : value_ { std::move(value) }
    {}

public:
    constexpr RuntimeRef<Value> 
        inner_search(Unit<Key>&&) noexcept
    {
        return value_;
    }

    constexpr RuntimeRef<const Value> 
        inner_search(Unit<Key>&&) const noexcept
    {
        return value_;
    }

private:
    Value value_;
};

template<wrapped_with<Pack> KeysPack, typename Value>
    requires (pack_size_v<KeysPack> > 0)
class MultyKVPair
{
public:
    constexpr MultyKVPair(KeysPack&&, Value&& value)
        : value_ { std::move(value) }
    {}

public:
    template<typename Key>
        requires pack_contains_t<KeysPack, Key>
    constexpr RuntimeRef<Value> 
        inner_search(Unit<Key>&& key) noexcept
    {
        return [this, &key]<typename... Keys>(Pack<Keys...>) -> Value& {
            return Overload {
                [this](Unit<Keys>&&) -> Value& {
                    return this->value_;
                }...
            }(std::move(key));
        }(KeysPack{});
    }

    template<typename Key>
        requires pack_contains_t<KeysPack, Key>
    constexpr RuntimeRef<const Value> 
        inner_search(Unit<Key>&& key) const noexcept
    {
        return [this, &key]<typename... Keys>(Pack<Keys...>) -> const Value& {
            return Overload {
                [this](Unit<Keys>&&) -> const Value& {
                    return this->value_;
                }...
            }(std::move(key));
        }(KeysPack{});
    }

private:
    Value value_;
};

template <typename... KVPairs>
class MetaMap : private KVPairs...
{
private:
    using KVPairs::inner_search...;

public:
    constexpr explicit MetaMap(KVPairs&&... pairs)
        : KVPairs(std::move(pairs))...
    {}

public:
    template<typename Self, typename Key>
    constexpr wrapped_with<std::optional> auto 
        find(this Self&& self, Unit<Key>&& key) noexcept
    {
        return std::forward<Self>(self)
            .static_find(std::move(key))
            .as_optional();
    }

    template<typename Self, typename Key>
    constexpr wrapped_with<StaticMaybe> auto 
        static_find(this Self&& self, Unit<Key>&& key) noexcept
    {
        #define RETRIEVAL_CALL_ \
            std::forward<Self>(self).inner_search(std::move(key))

        if constexpr (requires { RETRIEVAL_CALL_; })
        {
            return StaticMaybe { 
                RETRIEVAL_CALL_
            };
        }
        else 
        {
            return StaticNone<None>{};
        }

        #undef RETRIEVAL_CALL_
    }
};

}

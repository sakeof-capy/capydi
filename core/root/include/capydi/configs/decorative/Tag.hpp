#ifndef TAG_CONFIG_HPP_
#define TAG_CONFIG_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/referencing/Reference.hpp"

#include <capymeta/algorithms/pack/Append.hpp>
#include <capymeta/algorithms/pack/PackMap.hpp>
#include <capymeta/primitives/Overload.hpp>

namespace capy::di
{

using TagType = std::size_t;

/// @cond HIDDEN

namespace hidden__
{
    template<TagType TagValue, CreationalConfig Decoratee>
    class TagDecorator;
}

//// @endcond

template<TagType TagValue>
struct Tag
{
    template<CreationalConfig Decoratee>
    using Decorator = hidden__::TagDecorator<TagValue, Decoratee>;
};

namespace hidden__
{
    template<TagType TagValue, CreationalConfig Decoratee>
    class TagDecorator
    {
    public:
        using KeysPack = resolution_keys_pack_t<Decoratee>;

        template<typename Key>
        struct TransformKey 
        {
            using type = meta::append_t<meta::ValueUnit<TagValue>, Key>;
        };

        template<typename Key>
        using transform_key_t = typename TransformKey<Key>::type;

    public:
        using CentralType = central_type_t<Decoratee>;
        using ResolutionKeysPack = meta::pack_map_t<KeysPack, TransformKey>;

    public:
        static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;

    public:
        template<typename... Args>
        constexpr Reference<CentralType> auto 
            do_resolve(Args&&... args) const
        {
            return [&, this]<typename... Keys>(meta::Pack<Keys...>&&) {
                return meta::Overload {
                    [this]<typename... Dependencies>(
                        transform_key_t<Keys>&&,
                        std::tuple<Dependencies...>& dependencies
                    ) {
                        return this->decoratee_.do_resolve(
                            Keys{}, 
                            dependencies
                        );
                    }...
                }(std::forward<Args>(args)...);
            }(KeysPack{});
        }

    private:
        Decoratee decoratee_;
    };
}

}

#endif // !TAG_CONFIG_HPP_

#ifndef TAG_CONFIG_HPP_
#define TAG_CONFIG_HPP_

#include "capydi/utilities/Overload.hpp"
#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/utilities/pack/Append.hpp"
#include "capydi/utilities/pack/PackMap.hpp"
#include "capydi/utilities/referencing/Reference.hpp"

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
            using type = append_t<ValueUnit<TagValue>, Key>;
        };

        template<typename Key>
        using transform_key_t = typename TransformKey<Key>::type;

    public:
        using CentralType = central_type_t<Decoratee>;
        using ResolutionKeysPack = pack_map_t<KeysPack, TransformKey>;

    public:
        static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;

    public:
        template<typename... Args>
        constexpr Reference<CentralType> auto 
            do_resolve(Args&&... args) const
        {
            return [&, this]<typename... Keys>(Pack<Keys...>&&) {
                return Overload {
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

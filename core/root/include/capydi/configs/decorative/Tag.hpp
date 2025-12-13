#ifndef TAG_CONFIG_HPP_
#define TAG_CONFIG_HPP_

#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "capydi/utilities/pack/Append.hpp"
#include "capydi/utilities/referencing/Reference.hpp"

#include <cstddef>

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


template<TagType TagValue>
struct TagKey {};

/// @cond HIDDEN

namespace hidden__
{
    template<
        typename Self,
        TagType TagValue, 
        CreationalConfig Decoratee, 
        typename ResolutionKey_
    >
    class TagUnit
    {
    public:
        using ResolutionKey = append_t<TagKey<TagValue>, ResolutionKey_>;

    protected:
        template<typename... Dependencies>
        constexpr Reference<central_type_t<Decoratee>> auto 
            do_resolve(
                ResolutionKey key, 
                std::tuple<Dependencies...>& dependencies
            ) const
        {
            return static_cast<const Self&>(*this)
                .decoratee()
                .do_resolve(
                    ResolutionKey_{}, 
                    dependencies
                );
        }
    };

    template<
        TagType TagValue, 
        CreationalConfig Decoratee,
        typename ResolutionKeysPack
    >
    class TagUnitAggregator;

    template<
        TagType TagValue, 
        CreationalConfig Decoratee,
        typename... ResolutionKeys
    >
    class TagUnitAggregator<TagValue, Decoratee, Pack<ResolutionKeys...>>
        : public TagUnit<
            /* Self             = */ TagUnitAggregator<
                TagValue, 
                Decoratee, 
                Pack<ResolutionKeys...>
            >,
            /* TagValue         = */ TagValue, 
            /* Decoratee        = */ Decoratee, 
            /* ResolutionKeys_  = */ResolutionKeys
        >...
    {
    protected:
        using Self = TagUnitAggregator<
            TagValue, 
            Decoratee, 
            Pack<ResolutionKeys...>
        >;

        using TagUnit<
            Self, TagValue, Decoratee, ResolutionKeys
        >::do_resolve...;

        using /* Pack<Pack<?>> */ ResolutionKeysPack = Pack<
            typename TagUnit<
                Self, TagValue, Decoratee, ResolutionKeys
            >::ResolutionKey...
        >; 

    public:
        constexpr const Decoratee& decoratee() const noexcept
        {
            return this->decoratee_;
        }

        Decoratee& decoratee() noexcept
        {
            return this->decoratee_;
        }
        
    private:
        Decoratee decoratee_;
    };

    template<TagType TagValue, CreationalConfig Decoratee>
    class TagDecorator 
        : public TagUnitAggregator<
            TagValue,
            Decoratee,
            resolution_keys_pack_t<Decoratee>
        >
    {};
}

//// @endcond

}

#endif // !TAG_CONFIG_HPP_
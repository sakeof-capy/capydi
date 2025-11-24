#ifndef TAG_CONFIG_HPP_
#define TAG_CONFIG_HPP_

#include "configs/concepts/CreationalConfig.hpp"
#include "utilities/pack/Append.hpp"
#include "utilities/referencing/Reference.hpp"
#include <cstddef>

namespace capy::di
{

using TagType = std::size_t;

namespace hidden__
{
    template<TagType TagValue, CreationalConfig Decoratee>
    class TagDecorator;
}

template<TagType TagValue>
struct Tag
{
    template<CreationalConfig Decoratee>
    using Decorator = hidden__::TagDecorator<TagValue, Decoratee>;
};


template<TagType TagValue>
struct TagKey {};

namespace hidden__
{
    /*
        We need this wo share reference to decoratee_
        among TagUnits safely
    */
    template<TagType TagValue, CreationalConfig Decoratee>
    class TagDecorateeHolder
    {
    protected:
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

    template<
        TagType TagValue, 
        CreationalConfig Decoratee, 
        typename ResolutionKey_
    >
    class TagUnit 
        : private virtual TagDecorateeHolder<TagValue, Decoratee>
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
            return this->decoratee().do_resolve(
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
        : public TagUnit<TagValue, Decoratee, ResolutionKeys>...
    {
    protected:
        using TagUnit<TagValue, Decoratee, ResolutionKeys>::do_resolve...;
        using /* Pack<Pack<?>> */ ResolutionKeysPack = Pack<
            typename TagUnit<
                TagValue, 
                Decoratee, 
                ResolutionKeys
            >::ResolutionKey...
        >; 
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

}

#endif // !TAG_CONFIG_HPP_
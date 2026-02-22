#ifndef CAPYDI_RESOLUTION_OVERRIDES_HPP_
#define CAPYDI_RESOLUTION_OVERRIDES_HPP_

#include <capymeta/concepts/WrappedWIth.hpp>
#include <capymeta/type_structures/StaticMaybe.hpp>
#include <capymeta/algorithms/pack/Contains.hpp>
#include <tuple>
#include <concepts>

namespace capy::di
{

template<typename... Overrides>
class ResolutionOverrides
{
private:
    using OverrideHasBeenUsedFlag = bool;
    
    template<typename Override>
    struct OverrideCell 
    {
        Override stored_override;
        mutable OverrideHasBeenUsedFlag used;
    };

    using ConfigsContainer = std::tuple<
        OverrideCell<Overrides>...
    >;

public:
    constexpr explicit ResolutionOverrides(Overrides&&... overrides)
        : overrides_ { OverrideCell {
            .stored_override = std::move(overrides), 
            .used = false,
        }...}
    {}

    template<typename Override>
    constexpr std::optional<Override> retrieve_override() const
    {
        using AllOverridesPack = meta::Pack<std::decay_t<Overrides>...>;

        if constexpr (meta::pack_contains_t<AllOverridesPack, Override>)
        {
            const OverrideCell<Override>& override_cell
                = std::get<OverrideCell<Override>>(this->overrides_);

            override_cell.used = true;
            
            return override_cell.stored_override;
        }
        else 
        {
            return std::nullopt;
        }
    }

    constexpr bool validate() const 
    {
        return std::apply([](auto&&... cells){
            return (cells.used && ...);
        }, this->overrides_);
    }

    constexpr void reset() const 
    {
        return std::apply([](auto&&... cells){
            ((cells.used = false), ...);
        }, this->overrides_);
    }

private:
    ConfigsContainer overrides_;
};

}

#endif // !CAPYDI_RESOLUTION_OVERRIDES_HPP_

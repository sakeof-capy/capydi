#ifndef CAPYDI_RESOLUTION_OVERRIDES_HPP_
#define CAPYDI_RESOLUTION_OVERRIDES_HPP_

#include <capymeta/concepts/WrappedWIth.hpp>
#include <capymeta/type_structures/StaticMaybe.hpp>
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
    struct OverrudeCell 
    {
        Override stored_override;
        mutable OverrideHasBeenUsedFlag used;
    };

    using ConfigsContainer = std::tuple<
        OverrideCell<Overrides>...
    >;

public:
    constexpr explicit ResolutionOverrides(Overrides&&... overrides)
        overrides_ { std::move(overrides)... }
    {}

    template<typename Override>
        requires requires(const ConfigsContainer& tuple) { std::get<OverrideCell<Override>>(tuple) }
    constexpr const Override&
        retrieve_override() const
    {
        const OverrideCell<Override>& override_cell
            = std::get<OverrideCell<Override>>(this->overrides_);

        override_cell.used = true;
        
        return override_cell;
    }

    constexpr bool validate() const 
    {
        return applied_overrides_count_ == sizeof...(Overrides);
    }

private:
    ConfigsContainer overrides_;
};

}

#endif // !CAPYDI_RESOLUTION_OVERRIDES_HPP_

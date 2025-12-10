#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

namespace capy::di
{

/// @cond HIDDEN

namespace hidden__
{
    template<typename Interface, typename Decoratee>
    class InterfaceDecorator;
}

//// @endcond

template<typename InterfaceType>
struct Interface
{
    template<typename Decoratee>
    using Decorator = hidden__::InterfaceDecorator<InterfaceType, Decoratee>;
};

/// @cond HIDDEN

namespace hidden__
{
    template<typename Interface, typename Decoratee>
    class InterfaceDecorator : public Decoratee
    {
    public:
        template<typename... Args>
        constexpr InterfaceDecorator(Args&&... args)
            : Decoratee(std::forward<Args>(args)...)
        {}

        // Additional interface-specific functionality can be added here
    };

}

//// @endcond

}

#endif // INTERFACE_HPP_

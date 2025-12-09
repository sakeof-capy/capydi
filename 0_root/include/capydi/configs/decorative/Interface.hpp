#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

namespace capy::di
{

namespace hidden__
{
    template<typename Interface, typename Decoratee>
    class InterfaceDecorator;
}

template<typename InterfaceType>
struct Interface
{
    template<typename Decoratee>
    using Decorator = hidden__::InterfaceDecorator<InterfaceType, Decoratee>;
};

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

#endif // INTERFACE_HPP_

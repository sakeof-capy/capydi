#include <iostream>

struct A;
struct B;

#include "capydi/utilities/pack/PackAlgorithm.hpp"
#include "capydi/utilities/referencing/Reference.hpp"
#include "capydi/configs/creational/Singleton.hpp"
#include "capydi/configs/creational/Constleton.hpp"
#include "capydi/configs/decorative/Tag.hpp"
#include "capydi/configs/concepts/CreationalConfig.hpp"
#include "Container.hpp"

using namespace capy::di;

// template<typename... Ts>
// void print_tuple(const std::tuple<Ts...>& t) {
//     std::cout << "(";
//     std::apply([](const auto&... elems) {
//         std::size_t n = 0;
//         ((std::cout << elems << (++n != sizeof...(elems) ? ", " : "")), ...);
//     }, t);
//     std::cout << ")";
// }

// struct A { 
//     int inner = 10;

//     constexpr static A create() {
//         return A{};
//     }

//     constexpr void modify(int new_inner)
//     {
//         this->inner = new_inner;
//     }
// };
// struct B { 
//     int inner = 12; 

//     constexpr static B create() {
//         return B{};
//     }

//     constexpr void modify(int new_inner)
//     {
//         this->inner = new_inner;
//     }
// };

// struct C
// {
//     const A& a_;
//     const B& b_;
//     int inner_ = 321;

//     constexpr C(const A& a, const B& b)
//         : a_(a), b_(b)
//     {}

//     constexpr static C create(const A& a, const B& b)
//     {
//         return C(a, b);
//     }

//     void do_job()
//     {
//         this->inner_ = this->inner_ == 321 ? 123 : 321;
//         std::cout 
//             << "Const: a = " << this->a_.inner 
//             << "; b = " << this->b_.inner 
//             << "; inner = " << this->inner_
//             << "." << std::endl;
//     }
// };

// struct Const
// {
//     const A& a_;
//     const B& b_;

//     constexpr Const(const A& a, const B& b)
//         : a_(a), b_(b)
//     {}

//     constexpr static Const create(const A& a, const B& b)
//     {
//         return Const (a, b);
//     }

//     constexpr void do_job() const
//     {
//         std::cout << "Const: a = " << this->a_.inner << "; b = " << this->b_.inner << "." << std::endl;
//     }
// };

// struct D
// {
//     const A& a_;
//     C& c_;

//     constexpr D(const A& a, C& c)
//         : a_(a), c_(c)
//     {}

//     constexpr static D create(const A& a, C& c)
//     {
//         return D (a, c);
//     }

//     constexpr void do_job()
//     {
//         std::cout << "D: a = " << this->a_.inner
//             << " c.inner = " << this->c_.inner_;

//         std::cout << "D::c.do_job:\n";
//         this->c_.do_job();
//         std::cout << "D: end <====" << std::endl;
//     }
// };

// template<typename Pointer_, typename Pointee>
// concept IsPointer = std::convertible_to<Pointer_, const Pointee*>;


// template<typename Reference_, typename Referent>
// concept IsReference = std::convertible_to<Reference_, const Referent&>;


// template<typename T, const T& Ref>
// struct CPR 
// {
// public:
//     constexpr operator const T&() const noexcept
//     {
//         return Ref;
//     }
// };

// constexpr auto make_int()
// {
//     static constexpr int a = 2;
//     return CPR<int, a>{};
// }

// #include "capydi/Resolution.hpp"

// int main() 
// {
//     using Type = std::expected<int, double>;
//     static_assert(hidden__::is_wrapped_with_expected<Type>, "FAILURE");
//     // static constexpr DI container {
//     //     Constleton<A>
//     //         ::with<Tag<2>>
//     //         // ::with< Interface<>         >
//     //         // ::with< ConstructionArgs<>  >
//     //     {},
//     //     Constleton<B>{},
//     //     Constleton<Const>{},
//     //     Singleton<C>{},
//     //     Singleton<D>{},
//     // };

//     // static constexpr const A& ref_a = container.resolve<const A, TagKey<2>>();
//     // std::cout << "resolved A = " << ref_a.inner << std::endl;
    
//     // static constexpr const B& ref_b = container.resolve<const B>();
//     // std::cout << "resolved B = " << ref_b.inner << std::endl;

//     return EXIT_SUCCESS;
// }


int main()
{
    static_assert(CreationalConfig<Singleton<int>>);
}
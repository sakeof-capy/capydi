#include <iostream>

struct A;
struct B;

#include "utilities/pack/PackAlgorithm.hpp"
#include "utilities/referencing/Reference.hpp"
#include "configs/creational/Singleton.hpp"
#include "configs/creational/Constleton.hpp"
#include "Container.hpp"

using namespace capy::di;

template<typename... Ts>
void print_tuple(const std::tuple<Ts...>& t) {
    std::cout << "(";
    std::apply([](const auto&... elems) {
        std::size_t n = 0;
        ((std::cout << elems << (++n != sizeof...(elems) ? ", " : "")), ...);
    }, t);
    std::cout << ")";
}

struct A { 
    int inner = 10;

    constexpr static A create() {
        return A{};
    }

    constexpr void modify(int new_inner)
    {
        this->inner = new_inner;
    }
};
struct B { 
    int inner = 12; 

    constexpr static B create() {
        return B{};
    }

    constexpr void modify(int new_inner)
    {
        this->inner = new_inner;
    }
};

struct C
{
    const A& a_;
    const B& b_;
    int inner_ = 321;

    constexpr C(const A& a, const B& b)
        : a_(a), b_(b)
    {}

    constexpr static C create(const A& a, const B& b)
    {
        return C(a, b);
    }

    void do_job()
    {
        this->inner_ = this->inner_ == 321 ? 123 : 321;
        std::cout 
            << "Const: a = " << this->a_.inner 
            << "; b = " << this->b_.inner 
            << "; inner = " << this->inner_
            << "." << std::endl;
    }
};

struct Const
{
    const A& a_;
    const B& b_;

    constexpr Const(const A& a, const B& b)
        : a_(a), b_(b)
    {}

    constexpr static Const create(const A& a, const B& b)
    {
        return Const (a, b);
    }

    constexpr void do_job() const
    {
        std::cout << "Const: a = " << this->a_.inner << "; b = " << this->b_.inner << "." << std::endl;
    }
};

struct D
{
    const A& a_;
    C& c_;

    constexpr D(const A& a, C& c)
        : a_(a), c_(c)
    {}

    constexpr static D create(const A& a, C& c)
    {
        return D (a, c);
    }

    constexpr void do_job()
    {
        std::cout << "D: a = " << this->a_.inner
            << " c.inner = " << this->c_.inner_;

        std::cout << "D::c.do_job:\n";
        this->c_.do_job();
        std::cout << "D: end <====" << std::endl;
    }
};

template<typename Pointer_, typename Pointee>
concept IsPointer = std::convertible_to<Pointer_, const Pointee*>;


template<typename Reference_, typename Referent>
concept IsReference = std::convertible_to<Reference_, const Referent&>;


template<typename T, const T& Ref>
struct CPR 
{
public:
    constexpr operator const T&() const noexcept
    {
        return Ref;
    }
};

constexpr auto make_int()
{
    static constexpr int a = 2;
    return CPR<int, a>{};
}

int main() 
{
    // static constexpr int a = 1;
    // using ptr = int*;
    // using ref = decltype(*&a);
    // using cpr = CPR<int, a>;
    // auto another_int_cpr = make_int();
    // static_assert(IsPointer<ptr, int>);
    // static_assert(IsReference<ref, int>);
    // static_assert(IsReference<cpr, int>);
    // static_assert(IsReference<decltype(another_int_cpr), int>);
    // // static_assert(IsReference<RuntimeRef<int>, int>);
    // int b = 3;
    // RuntimeRef<const int> rref = b;
    // // int& rrefc = rref;
    // const int& crrefc = rref;
    // static_assert(IsReference<RuntimeRef<const int>, int>);

    static constexpr DI container {
        Constleton<A>{},
        Constleton<B>{},
        Constleton<Const>{},
        Singleton<C>{},
        Singleton<D>{},
    };

    static constexpr const A& ref_a = container.resolve<const A>();
    std::cout << "resolved A = " << ref_a.inner << std::endl;
    
    static constexpr const B& ref_b = container.resolve<const B>();
    std::cout << "resolved B = " << ref_b.inner << std::endl;

    static constexpr const Const& ref_const = container.resolve<const Const>();
    std::cout << "1. resolved Const = " << ref_const.a_.inner << ", " << ref_const.b_.inner << std::endl;
    ref_const.do_job();
    std::cout << "2. resolved Const = " << ref_const.a_.inner << ", " << ref_const.b_.inner << std::endl;

    C& ref_c = container.resolve<C>();
    std::cout << "1. resolved C = " << ref_c.a_.inner << ", " << ref_c.b_.inner << std::endl;
    ref_c.do_job();
    std::cout << "2. resolved C = " << ref_c.a_.inner << ", " << ref_c.b_.inner << std::endl;
    
    D& ref_d = container.resolve<D>();
    ref_d.do_job();

    return EXIT_SUCCESS;
}

// #include <iostream>

// template<typename T>
// struct TypeId {};

// struct AA
// {
//     void method(TypeId<AA> a)
//     {
//         std::cout << "Method AA" << std::endl;
//     }
// };

// struct BB
// {
//     void method(TypeId<BB> a)
//     {
//         std::cout << "Method BB" << std::endl;
//     }
// };

// struct Combo: AA, BB
// {
//     using AA::method;
//     using BB::method;
// };

// int main()
// {
//     Combo combo{};
//     combo.method(TypeId<BB>{});
//     return 0;
// }

/*

capy::di::valued_pack_for(Pack<Elements ...>&&, Handler&&) 
[
    with 
        Handler = ...; PackElements = {const A&, const B&}; 
        hidden__::valued_pack_for_result<Handler, PackElements ...> = std::tuple<
            ConstexprRef<A, instance>, 
            ConstexprRef<B, instance> 
        >
]
    (<lambda closure object>capy::di::DI<capy::di::Constleton<A>, capy::di::Constleton<B>, capy::di::Constleton<Const> >::resolve<const Const>() const::<lambda(capy::di::Unit<T&>)>{((const capy::di::DI<capy::di::Constleton<A>, capy::di::Constleton<B>, capy::di::Constleton<Const> >*)this)})

*/
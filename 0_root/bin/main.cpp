#include <iostream>

struct A;
struct B;

#include "utilities/PackAlgorithm.hpp"
#include "configs/Singleton.hpp"
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

/*
### valued_pack_for test:

std::cout << "Hello, World!\n" << std::flush;

    auto lam = []<typename T>(Unit<T>) -> T {
        std::cout << "PRINTING: " << typeid(T).name() << std::endl;
        return T{} + 1;
    };

    // using a = std::invoke_result_t<decltype(lam), Unit<int>>;


    using to_test = hidden__::valued_pack_for_result<decltype(lam), int, long long, double, float>;

    auto tup = valued_pack_for(Pack<int, double, float>{}, std::move(lam));

    std::cout << "___________________________________" << std::endl;

    // std::cout << typeid(a).name() << std::endl;
    print_tuple(tup);
*/

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
    A& a_;
    const B& b_;

    constexpr C(A& a, const B& b)
        : a_(a), b_(b)
    {}

    constexpr static C create(A& a, const B& b)
    {
        return C(a, b);
    }

    constexpr void do_job()
    {
        this->a_.modify(133);
    }
};

struct D
{
    const A& a_;
    B& b_;

    constexpr D(const A& a, B& b)
        : a_(a), b_(b)
    {}

    constexpr static D create(const A& a, B& b)
    {
        return D (a, b);
    }

    constexpr void do_job()
    {
        this->b_.modify(134);
    }
};

int main() 
{
    DI container {
        Singleton<A>{},
        Singleton<B>{},
        Singleton<C>{},
        Singleton<D>{},
    };

    A& ref_a = container.resolve<A>();
    std::cout << "resolved A = " << ref_a.inner << std::endl;
    
    B& ref_b = container.resolve<B>();
    std::cout << "resolved B = " << ref_b.inner << std::endl;

    C& ref_c = container.resolve<C>();
    std::cout << "1. resolved C = " << ref_c.a_.inner << ", " << ref_c.b_.inner << std::endl;
    ref_c.do_job();
    std::cout << "2. resolved C = " << ref_c.a_.inner << ", " << ref_c.b_.inner << std::endl;
    
    D& ref_d = container.resolve<D>();
    std::cout << "1. resolved D = " << ref_d.a_.inner << ", " << ref_d.b_.inner << std::endl;
    ref_d.do_job();
    std::cout << "2. resolved D = " << ref_d.a_.inner << ", " << ref_d.b_.inner << std::endl;

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
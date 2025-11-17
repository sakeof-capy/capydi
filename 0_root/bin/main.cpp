#include <iostream>

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
};
struct B { 
    int inner = 12; 
    constexpr static B create() {
        return B{};
    }
};

struct C
{
    A& a_;
    B& b_;

    constexpr C(A& a, B& b)
        : a_(a), b_(b)
    {}

    constexpr static C create(A& a, B& b)
    {
        return C (a, b);
    }
};

int main() 
{
    DI container {
        Singleton<A>{},
        Singleton<B>{},
        Singleton<C>{},
    };

    A& ref_a = container.resolve<A>();
    std::cout << "resolved A = " << ref_a.inner << std::endl;
    
    B& ref_b = container.resolve<B>();
    std::cout << "resolved B = " << ref_b.inner << std::endl;

    C& ref_c = container.resolve<C>();
    std::cout << "resolved C = " << ref_c.a_.inner << ", " << ref_c.b_.inner << std::endl;

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
/**
 * @file asdasd.cpp
 * @brief Test C++20 concept documentation with Doxygen
 */

/**
 * @concept MyConcept
 * @brief Example concept: types providing a `doSomething()` method returning int.
 *
 * Models of MyConcept must provide:
 * - `int doSomething();`
 */
template<typename T>
concept MyConcept = requires(T t) {
    { t.doSomething() } -> std::same_as<int>;
};

/**
 * @class ImplA
 * @brief Example class implementing MyConcept
 * @concept MyConcept
 */
class ImplA {
public:
    int doSomething() {
        return 42;
    }
};

/**
 * @class ImplB
 * @brief Another implementer of MyConcept
 * @concept MyConcept
 */
class ImplB {
public:
    int doSomething() {
        return 7;
    }
};

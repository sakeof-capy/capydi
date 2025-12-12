/**
 * @mainpage Capydi: A Elegant, Zero-Cost Dependency Injection Framework for C++23
 * 
 * @section intro Introduction
 * 
 * **Capydi** is a modern, header-only dependency injection framework designed for C++23
 * and beyond. It leverages compile-time concepts, template metaprogramming, and constexpr
 * to provide type-safe, zero-overhead dependency resolution with an elegant API.
 * 
 * The framework emphasizes:
 * - **Type Safety**: Concepts enforce correct configuration and usage patterns at compile time
 * - **Zero Overhead**: All resolution is compile-time; no runtime polymorphism or reflection
 * - **Elegance**: Clean, intuitive API that reads naturally while maintaining power
 * - **Flexibility**: Support for both creational patterns and decorator/chainable transformations
 * 
 * @section architecture High-Level Architecture
 * 
 * @dot
 * digraph Capydi_Architecture {
 *     rankdir=TB;
 *     
 *     subgraph cluster_user {
 *         label = "User Code";
 *         User [shape=box, label="Application\nCode"];
 *     }
 *     
 *     subgraph cluster_core {
 *         label = "Core DI Framework";
 *         DI [shape=box, label="DI<Configs...>"];
 *         Classify [shape=box, label="Config\nClassifier"];
 *         Creational [shape=box, label="Creational\nDispatcher"];
 *         Chainable [shape=box, label="Chainable\nDispatcher"];
 *     }
 *     
 *     subgraph cluster_concepts {
 *         label = "Concept Layer";
 *         ConfigConcept [shape=ellipse, label="DiConfig"];
 *         CreationalConcept [shape=ellipse, label="CreationalConfig"];
 *         ChainableConcept [shape=ellipse, label="ChainableConfig"];
 *         RefConcept [shape=ellipse, label="Reference"];
 *         ResConcept [shape=ellipse, label="Resolution"];
 *     }
 *     
 *     User -> DI;
 *     DI -> Classify;
 *     Classify -> Creational;
 *     Classify -> Chainable;
 *     
 *     DI -> ConfigConcept;
 *     Creational -> CreationalConcept;
 *     Chainable -> ChainableConcept;
 *     Chainable -> RefConcept;
 *     DI -> ResConcept;
 * }
 * @enddot
 * 
 * @section core_concepts Core Concepts
 * 
 * ### DiConfig
 * A marker concept that all configurations must satisfy. Configurations are classified
 * into two categories:
 * 
 * - **CreationalConfig**: Handles instantiation of objects (factories, constructors, etc.)
 * - **ChainableConfig**: Handles transformation/decoration of objects (logging, caching, etc.)
 * 
 * ### Reference
 * An abstraction for referring to objects that supports both runtime (@c RuntimeRef) and
 * compile-time (@c ConstexprRef) scenarios. All transformations in the decorator chain
 * work through the Reference concept.
 * 
 * ### Resolution
 * The result type of a dependency query: @c std::expected<Reference<T>, Error>.
 * Enables elegant error handling without exceptions.
 * 
 * @section usage Basic Usage
 * 
 * @subsection creation Creating a Container
 * 
 * @code
 * #include <capydi/Container.hpp>
 * #include <capydi/configs/creational/Factory.hpp>
 * #include <capydi/configs/chainable/Decorator.hpp>
 * 
 * // Define your service
 * class Logger {
 * public:
 *     void log(const std::string& msg) { std::cout << msg << '\n'; }
 * };
 * 
 * class MyService {
 * public:
 *     void doWork() { std::cout << "Working...\n"; }
 * };
 * 
 * // Create a container
 * auto di = capy::di::DI(
 *     capy::di::Factory<Logger>{},
 *     capy::di::Factory<MyService>{},
 *     capy::di::Decorator<LoggingDecorator>{}
 * );
 * 
 * // Resolve dependencies
 * auto result = di.resolve<LoggerKey>();
 * if (result.has_value()) {
 *     auto& logger = result.value().get();
 *     logger.log("Dependency resolved!");
 * }
 * @endcode
 * 
 * @subsection decorators Using Decorators
 * 
 * Decorators form a transformation pipeline:
 * 
 * @code
 * class CachingDecorator {
 * public:
 *     using RelatedEntity = MyService;
 *     using RelatedKey = CacheKey;
 *     static constexpr ConfigType CONFIG_TYPE = ConfigType::CHAINABLE;
 *     
 *     capy::di::Reference<MyService> auto pipe(
 *         capy::di::Reference<MyService> auto service
 *     ) const {
 *         // Return a cached version of service
 *         return capy::di::RuntimeRef(cached_instance);
 *     }
 * };
 * 
 * auto di = capy::di::DI(
 *     Factory<MyService>{},
 *     CachingDecorator{},
 *     LoggingDecorator{}
 * );
 * @endcode
 * 
 * @section design Design Philosophy
 * 
 * Capydi is built on several core principles:
 * 
 * 1. **Compile-Time First**: Leverage C++20 concepts and templates to catch errors early
 * 2. **Zero-Cost Abstraction**: No runtime overhead for what happens at compile time
 * 3. **Explicit Over Implicit**: Users should understand how dependencies flow
 * 4. **Orthogonal Concerns**: Separate creation (creational) from decoration (chainable)
 * 5. **Type Safety Without Reflection**: Use concepts instead of runtime type information
 * 
 * @section performance Performance Characteristics
 * 
 * - **Compile Time**: Configuration processing and type checking happen at compile time.
 *   Larger configurations may increase build times, but this is a one-time cost.
 * - **Runtime**: Container construction and resolution are typically constexpr-compatible,
 *   allowing compilation of the entire DI pipeline into static initializers.
 * - **Binary Size**: Template instantiation may increase binary size for complex configurations,
 *   but patterns (like Pack operations) are highly optimizable.
 * 
 * @section advanced Advanced Topics
 * 
 * ### Custom Configurations
 * 
 * Implement your own configs by satisfying CreationalConfig or ChainableConfig concepts:
 * 
 * @code
 * struct MyCustomConfig {
 *     using RelatedKey = MyKey;
 *     using RelatedEntity = MyType;
 *     static constexpr ConfigType CONFIG_TYPE = ConfigType::CREATIONAL;
 *     
 *     // For creational: provide creation logic
 *     // For chainable: implement pipe(Reference<RelatedEntity> auto) method
 * };
 * @endcode
 * 
 * ### Pack-Based Type Manipulation
 * 
 * The utilities in @c utilities/pack/ enable compile-time type filtering and rebinding:
 * 
 * @code
 * using AllConfigs = Pack<ConfigA, ConfigB, ConfigC>;
 * using CreationalOnly = filter_t<AllConfigs, IsCreationalConfig>;
 * using Dispatchers = rebind_pack_t<CreationalOnly, CreationalConfigDispatcher>;
 * @endcode
 * 
 * @section modules Key Modules
 * 
 * - **Container** (@ref Container.hpp): The main DI container class
 * - **Concepts** (@ref ChainableConfig.hpp, @ref CreationalConfig.hpp, @ref DiConfig.hpp): 
 *   Core constraint definitions
 * - **Utilities** (@ref Pack.hpp, @ref Reference.hpp, @ref RuntimeRef.hpp, @ref ConstexprRef.hpp):
 *   Compile-time metaprogramming and reference abstractions
 * - **Dispatchers** (CreationalConfigDispatcher, ChainableConfigDispatcher): 
 *   Internal routing of resolution requests
 * 
 * @section faq Frequently Asked Questions
 * 
 * **Q: Why concepts instead of virtual functions?**
 * 
 * A: Concepts enforce constraints at compile time without runtime overhead. They enable
 * the same expressiveness as runtime polymorphism with zero cost.
 * 
 * **Q: Can I mix runtime and compile-time DI?**
 * 
 * A: Not directly. Capydi is optimized for the compile-time case where the entire
 * dependency graph is known. For hybrid approaches, consider factory functions that
 * accept runtime parameters.
 * 
 * **Q: How do I handle optional dependencies?**
 * 
 * A: Use `std::optional<Reference<T>>` or wrap resolution in `std::expected`, then
 * check for presence before use.
 * 
 * @section see_also See Also
 * 
 * - @ref capy::di::DI "DI Container"
 * - @ref capy::di::Resolution "Resolution Concept"
 * - @ref capy::di::Reference "Reference Concept"
 * - @ref capy::di::Error "Error Codes"
 * 
 * @section license License
 * 
 * Capydi is released under the MIT License. See LICENSE.md for details.
 * 
 * @section author Author & Contributors
 * 
 * Created by the Capydi team. Contributions are welcome!
 */

// This file is documentation-only and intentionally empty after the comment block.

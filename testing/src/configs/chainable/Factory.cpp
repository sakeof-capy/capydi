#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include "hierarchies/Operations.hpp"

#include <capydi/Container.hpp>
#include <capydi/configs/creational/Singleton.hpp>
#include <capydi/configs/chainable/Observer.hpp>
#include <capydi/configs/decorative/Tag.hpp>
#include <capydi/configs/inputs/TagInput.hpp>
#include <capydi/configs/decorative/Interface.hpp>
#include <capydi/configs/decorative/DependencyTags.hpp>
#include <catch2/catch_test_macros.hpp>
#include <unordered_map>

using namespace capy::di;
using namespace capy::di::operations;
using namespace capy::meta;


enum class FactoryError
{
    REGISTRATION_NOT_FOUND,
    RESOLUTION_ERROR,
};

enum class RegistryError
{
    REGISTRATION_ALREADY_PRESENT,
};

class ValueFactory
{
public:
    Resolution<IValue, FactoryError> auto resolve(int key) const
    {
        auto retrieval_iterator = this->registry_.find(key);

        if (retrieval_iterator == this->registry_.cend()) [[unlikely]]
        {
            return std::expected<RuntimeRef<IValue>, FactoryError> {
                std::unexpected { FactoryError::REGISTRATION_NOT_FOUND }
            };
        }

        auto resolution = retrieval_iterator->second.resolve();

        return resolution
            .transform_error([](Error error) {
                return FactoryError::RESOLUTION_ERROR;
            });
    }

    std::expected<void, RegistryError> register_value(
        int key, 
        DynamicResolver<IValue>&& resolver
    ) {
        if (this->registry_.find(key) != this->registry_.cend()) [[unlikely]]
        {
            return std::unexpected { 
                RegistryError::REGISTRATION_ALREADY_PRESENT 
            };
        }

        this->registry_.emplace(key, std::move(resolver));
        return {};
    }

public:
    static ValueFactory create()
    {
        return {};
    }

private:
    std::unordered_map<int, DynamicResolver<IValue>> registry_;
};

TEST_CASE("factories:basic_registration") 
{
    const DI container {
        Singleton<ValueFactory>{}
            .with<Tag>("factory-tag"),

        Singleton<Value1>{}
            .with<Interface>(Unit<IValue>{})
            .with<Tag>("value1-tag"),

        OnObjectCreated{
            Unit<ValueFactory>{}, 
            DiAction {
                .action = [](
                    ValueFactory& factory, 
                    DynamicResolver<IValue> value_resolver
                ) {
                    factory.register_value(
                        123, 
                        std::move(value_resolver)
                    );
                },
                .dependency_tags = std::array {
                    DependencyTagPair { 0, "factory-tag" },
                    DependencyTagPair { 1, "value1-tag" },
                },
            },
        }.with<Tag>("factory-tag"),
    };


    Resolution<ValueFactory, Error> auto 
        factory_resolution = container.resolve<ValueFactory>(std::tuple { TagInput {
            "factory-tag"
        }});
    
    REQUIRE(factory_resolution.has_value());

    ValueFactory& factory = factory_resolution.value();

    Resolution<IValue, FactoryError> auto 
        value_resolution = factory.resolve(123);

    REQUIRE(value_resolution.has_value());

    IValue& value = value_resolution.value();

    REQUIRE(value.get_value() == Value1::VALUE);
}

TEST_CASE("factories:several_factories")
{
    static constexpr int FACTORY1_VALUE1_KEY = 123;
    static constexpr int FACTORY1_VALUE3_KEY = 234;
    static constexpr int FACTORY2_VALUE1_KEY = 1234;
    static constexpr int NON_EXISTANT_KEY = 12413413;

    const DI container {
        Singleton<ValueFactory>{}
            .with<Tag>("factory1-tag"),

        Singleton<ValueFactory>{}
            .with<Tag>("factory2-tag"),

        Singleton<Value1>{}
            .with<Interface>(Unit<IValue>{})
            .with<Tag>("value1-tag"),

        Singleton<Value3>{}
            .with<Interface>(Unit<IValue>{})
            .with<Tag>("value3-tag"),

        /* 1st Factory registration */
        OnObjectCreated {
            Unit<ValueFactory>{}, 
            DiAction {
                .action = [](
                    ValueFactory& factory, 
                    DynamicResolver<IValue> value_resolver
                ) {
                    factory.register_value(
                        FACTORY1_VALUE1_KEY, 
                        std::move(value_resolver)
                    );
                },
                .dependency_tags = std::array {
                    DependencyTagPair { 0, "factory1-tag" },
                    DependencyTagPair { 1, "value1-tag" },
                },
            },
        }.with<Tag>("factory1-tag"),

        /* 1st Factory registration */
        OnObjectCreated{
            Unit<ValueFactory>{}, 
            DiAction {
                .action = [](
                    ValueFactory& factory, 
                    DynamicResolver<IValue> value_resolver
                ) {
                    factory.register_value(
                        FACTORY1_VALUE3_KEY, 
                        std::move(value_resolver)
                    );
                },
                .dependency_tags = std::array {
                    DependencyTagPair { 0, "factory1-tag" },
                    DependencyTagPair { 1, "value3-tag" },
                },
            },
        }.with<Tag>("factory1-tag"),

        /* 2nd Factory registration */
        OnObjectCreated{
            Unit<ValueFactory>{},
            DiAction {
                .action = [](
                    ValueFactory& factory, 
                    DynamicResolver<IValue> value_resolver
                ) {
                    factory.register_value(
                        FACTORY2_VALUE1_KEY, 
                        std::move(value_resolver)
                    );
                },
                .dependency_tags = std::array {
                    DependencyTagPair { 0, "factory2-tag" },
                    DependencyTagPair { 1, "value1-tag" },
                },
            },
        }.with<Tag>("factory2-tag"),
    };

    Resolution<ValueFactory, Error> auto 
        factory1_resolution = container.resolve<ValueFactory>(std::tuple { TagInput {
            "factory1-tag"
        }});

    Resolution<ValueFactory, Error> auto 
        factory2_resolution = container.resolve<ValueFactory>(std::tuple { TagInput {
            "factory2-tag"
        }});
    
    REQUIRE(factory1_resolution.has_value());
    REQUIRE(factory2_resolution.has_value());

    ValueFactory& factory1 = factory1_resolution.value();
    ValueFactory& factory2 = factory2_resolution.value();

    SECTION("factory1:both_registrations")
    {
        Resolution<IValue, FactoryError> auto 
            value1_resolution = factory1.resolve(FACTORY1_VALUE1_KEY);
        Resolution<IValue, FactoryError> auto 
            value3_resolution = factory1.resolve(FACTORY1_VALUE3_KEY);

        REQUIRE(value1_resolution.has_value());
        REQUIRE(value3_resolution.has_value());

        IValue& value1 = value1_resolution.value();
        IValue& value3 = value3_resolution.value();

        REQUIRE(value1.get_value() == Value1::VALUE);
        REQUIRE(value3.get_value() == Value3::VALUE);
    }

    SECTION("factory2:only_one_registration")
    {
        Resolution<IValue, FactoryError> auto 
            value1_resolution = factory2.resolve(FACTORY2_VALUE1_KEY);
        Resolution<IValue, FactoryError> auto 
            value3_resolution = factory2.resolve(FACTORY1_VALUE3_KEY);

        REQUIRE(value1_resolution.has_value());
        REQUIRE(!value3_resolution.has_value());

        IValue& value1 = value1_resolution.value();

        REQUIRE(value1.get_value() == Value1::VALUE);
    }

    SECTION("both_factories:non_existant_key")
    {
        Resolution<IValue, FactoryError> auto 
            factory1_non_existant_resolution = factory1.resolve(NON_EXISTANT_KEY);

        Resolution<IValue, FactoryError> auto 
            factory2_non_existant_resolution = factory2.resolve(NON_EXISTANT_KEY);

        REQUIRE(!factory1_non_existant_resolution.has_value());
        REQUIRE(!factory2_non_existant_resolution.has_value());
    }
}

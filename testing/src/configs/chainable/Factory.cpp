#define CATCH_CONFIG_RUNTIME_STATIC_REQUIRE
#include "hierarchies/Operations.hpp"

#include <capydi/Container.hpp>
#include <capydi/configs/creational/Singleton.hpp>
#include <capydi/configs/chainable/OnObjectCreated.hpp>
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

template<typename ActionType, std::size_t TAG_OVERRIDES_COUNT>
struct ActionConfig
{
    ActionType action;
    std::array<DependencyTagPair, TAG_OVERRIDES_COUNT> args_tags;
};


TEST_CASE("factories") 
{
    const DI container {
        Singleton<ValueFactory>{}
            .with<Tag>("factory-tag"),

        Singleton<Value1>{}
            .with<Interface>(Unit<IValue>{}),
            // .with<Tag>("value1-tag"),
        // Singleton<Value3>{}
        //     .with<Interface>(Unit<IValue>{})
        //     .with<Tag>("value3-tag"),

        OnObjectCreated{
            Unit<ValueFactory>{}, 
            [](ValueFactory& factory, DynamicResolver<IValue>&& value_resolver) {
                factory.register_value(123, std::move(value_resolver));
            }
        }
        .with<Tag>("factory-tag")
    };

}

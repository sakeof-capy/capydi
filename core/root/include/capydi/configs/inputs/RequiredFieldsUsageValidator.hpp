#ifndef CAPYDI_REQUIRED_FIELDS_USAGE_VALIDATOR_HPP_
#define CAPYDI_REQUIRED_FIELDS_USAGE_VALIDATOR_HPP_

#include <capymeta/concepts/WrappedWIth.hpp>
#include <vector>

namespace capy::di
{

class RequiredFieldUsageObserver
{
public:
    constexpr explicit RequiredFieldUsageObserver(const bool& used)
        : used_ { used }
    {}

public:
    constexpr bool validate() const
    {
        return this->used_;
    }

private:
    const bool& used_;
};

class RequiredFieldsUsageValidator
{
public:
    constexpr void register_required_field(
        RequiredFieldUsageObserver&& observer
    ) {
        this->observers_.push_back(std::move(observer));
    }

    constexpr bool validate() const noexcept
    {
        bool validation_result = true;

        for (const RequiredFieldUsageObserver& observer : this->observers_)
        {
            validation_result = validation_result && observer.validate();
        }

        return validation_result;
    }

private:
    std::vector<RequiredFieldUsageObserver> observers_;
};

template<typename Type>
class RequiredField
{
public:
    constexpr explicit RequiredField(
        Type&& data, 
        RequiredFieldsUsageValidator& validator
    )
        : data_ { std::move(data) }
        , used_ { false }
    {
        validator.register_required_field(RequiredFieldUsageObserver {
            this->used_
        });
    }

public:
    constexpr Type& value() noexcept
    {
        this->used_ = true;
        return this->data_; 
    }

    constexpr const Type& value() const noexcept
    {
        this->used_ = true;
        return this->data_;
    }

private:
    Type data_;
    mutable bool used_;
};

class ValidatableInputBase
{
public:
    constexpr bool validate() const noexcept
    {
        return this->validator_.validate();
    }

protected:
    constexpr RequiredFieldsUsageValidator& fields_validator() noexcept
    {
        return this->validator_;
    }

private:
    RequiredFieldsUsageValidator validator_;
};

}

#endif // !CAPYDI_REQUIRED_FIELDS_USAGE_VALIDATOR_HPP_

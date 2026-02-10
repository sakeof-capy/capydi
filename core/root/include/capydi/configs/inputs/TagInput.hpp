#ifndef CAPYDI_TAG_INPUT_HPP_
#define CAPYDI_TAG_INPUT_HPP_

#include "RequiredFieldsUsageValidator.hpp"

namespace capy::di
{

struct TagInput : ValidatableInputBase
{
public:
    constexpr explicit TagInput(std::size_t tag_val)
        : ValidatableInputBase{}
        , tag { std::move(tag_val), this->fields_validator() }
    {}

public:
    RequiredField<std::size_t> tag;
};

}

#endif // !CAPYDI_TAG_INPUT_HPP_

#ifndef CAPYDI_TAG_INPUT_HPP_
#define CAPYDI_TAG_INPUT_HPP_

#include "RequiredFieldsUsageValidator.hpp"

namespace capy::di
{

using tag_t = const char*;

struct TagInput : ValidatableInputBase
{
public:
    constexpr explicit TagInput(tag_t tag_val)
        : ValidatableInputBase{}
        , tag { std::move(tag_val), this->fields_validator() }
    {}

public:
    RequiredField<tag_t> tag;
};

}

#endif // !CAPYDI_TAG_INPUT_HPP_

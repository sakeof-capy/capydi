#ifndef I_TOKENIZER_HPP_
#define I_TOKENIZER_HPP_

#include <optional>

template<typename TokenType_>
class ITokenizer
{
public:
    using TokenType = TokenType_;

public:
    virtual ~ITokenizer() = default;

public:
    virtual std::optional<TokenType> next_token() noexcept = 0;
};

#endif // !I_TOKENIZER_HPP_
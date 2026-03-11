#include "DelimFileReader.hpp"
#include <fstream>
#include <iterator>
#include <filesystem>
#include <algorithm>

DelimFileReader::DelimFileReader
(
    const PathType& file_path,
    const DelimsType& delims,
    const size_t average_token_size
)
    : file_(file_path, std::ios::in)
    , iterator_(file_)
    , delims_(delims)
    , average_token_size_(average_token_size)
{
    if (!file_.is_open())
    {
        throw std::ios_base::failure("File not found or cannot be opened.");
    }
}

auto DelimFileReader::next_token() noexcept -> std::optional<TokenType>
{
    const IteratorType end;

    if (iterator_ == end)
    {
        return std::nullopt;
    }

    TokenType token;
    token.reserve(average_token_size_);

    for (; iterator_ != end && !is_delim(*iterator_); ++iterator_)
    {
        token.push_back(*iterator_);
    }

    if (iterator_ != end)
    {
        ++iterator_;
    }

    return token;
}

bool DelimFileReader::is_delim(const FileCharType character) const noexcept
{
    return std::find(delims_.begin(), delims_.end(), character) != delims_.cend();
}
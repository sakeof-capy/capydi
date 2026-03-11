#ifndef DELIM_FILE_READER_
#define DELIM_FILE_READER_

#include "ITokenizer.hpp"
#include "FileSystem.hpp"
#include <string>
#include <filesystem>
#include <fstream>
#include <iterator>

class DelimFileReader : public ITokenizer<FileSystem::FileContentType>
{
private:
    using BaseType = ITokenizer<FileSystem::FileContentType>;

public:
    using PathType = FileSystem::PathType;
    using TokenType = typename BaseType::TokenType;
    using FileCharType = FileSystem::FileCharType;
    using DelimsType = TokenType;

private:
    using IteratorType = std::istreambuf_iterator<FileCharType>;

public:
    DelimFileReader(const PathType& file_path, const DelimsType& delims, const std::size_t average_token_size = 0u);
    ~DelimFileReader() override = default;

public:
    std::optional<TokenType> next_token() noexcept override;

private:
    bool is_delim(const FileCharType character) const noexcept;

private:
    std::ifstream file_;
    IteratorType iterator_;
    DelimsType delims_;
    std::size_t average_token_size_;
};

#endif // ! DELIM_FILE_READER_
#ifndef FILE_ITERATOR_HPP_
#define FILE_ITERATOR_HPP_

#include "FileSystem.hpp"
#include <optional>
#include <filesystem>
#include <regex>

class FileIterator
{
private:
    using IteratorType = std::filesystem::recursive_directory_iterator;
public:
    using PathType = typename FileSystem::PathType;
    using PathStringType = typename FileSystem::PathStringType;

#ifdef USE_WIDE_CHAR_PATH
    using RegexType = std::wregex;
#else
    using RegexType = std::regex;
#endif 

public:
    FileIterator(const PathType& folder_path, const PathStringType& regex);

public:
    std::optional<PathType> next_file() noexcept;

private:
    void skip_all_unneeded() noexcept;
    bool is_needed(const PathType& path) const noexcept;

private:
    IteratorType iterator_;
    RegexType regular_expression_;
};

#endif // !FILE_ITERATOR_HPP_

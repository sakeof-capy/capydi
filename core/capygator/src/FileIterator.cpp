#include "FileIterator.hpp"

FileIterator::FileIterator(const PathType& folder_path, const PathStringType& regex)
    : iterator_(folder_path)
    , regular_expression_(regex)
{
    skip_all_unneeded();
}

auto FileIterator::next_file() noexcept -> std::optional<PathType>
{
    IteratorType end;

    if (iterator_ == end)
    {
        return std::nullopt;
    }

    PathType next_file = *iterator_++;
    skip_all_unneeded();

    return next_file;
}

void FileIterator::skip_all_unneeded() noexcept
{
    for
    (
        IteratorType end;
        iterator_ != end && !is_needed(*iterator_);
        ++iterator_
    );
}

bool FileIterator::is_needed(const PathType& path) const noexcept
{
    using std::filesystem::is_directory;
    using std::regex_search;

    return !is_directory(path) &&
            regex_search(path.native(), regular_expression_);
}
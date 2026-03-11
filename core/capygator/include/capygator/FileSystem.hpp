#ifndef FILE_SYSTEM_HPP_
#define FILE_SYSTEM_HPP_
#include <filesystem>
#include <string>

namespace FileSystem
{

using PathType = std::filesystem::path;
using PathStringType = typename PathType::string_type;
using PathCharType = typename PathStringType::value_type;
using FileContentType = std::string;
using FileCharType = typename FileContentType::value_type;

}

#if defined(_WIN32) && !defined(UNICODE)
    #define USE_WIDE_CHAR_PATH
    #define TO_PATH_STR(str) L##str
#else
    #define TO_PATH_STR(str) str
#endif

#endif // !FILE_SYSTEM_HPP_

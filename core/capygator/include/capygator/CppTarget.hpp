#ifndef CPPTARGET_HPP_
#define CPPTARGET_HPP_

#include <vector>
#include "FileSystem.hpp"

class CppTarget
{
public:
    void include(const FileSystem::PathType& file_contents)
    {
        includes_.push_back(file_contents);
    }

    void add_subscriber_type(FileSystem::FileContentType&& subscriber_type)
    {
        subscriber_types_.push_back(subscriber_type.substr(0, subscriber_type.size() - 1));
    }

    FileSystem::FileContentType to_file_content() const
    {
        const FileSystem::FileContentType namespace_name = "DI";
        const FileSystem::FileContentType subscribe_all_fn_begin = "\nvoid subscribe_all_dependencies()\n{\n";
        FileSystem::FileContentType file_content;

        for (const auto& include_path: includes_)
        {
            file_content +=  "#include \"" + include_path.string() + "\"\n";
        }

        file_content += subscribe_all_fn_begin;

        for (const auto& subscriber_type: subscriber_types_)
        {
            file_content += "\t" + namespace_name + "::subscribe_" + subscriber_type + "();\n";
        }

        file_content += "}\n";

        return file_content;
    }

private:
    std::vector<FileSystem::PathType> includes_;
    std::vector<FileSystem::FileContentType> subscriber_types_;
};

#endif //CPPTARGET_HPP_

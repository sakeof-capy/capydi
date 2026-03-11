#ifndef HPPTARGET_HPP_
#define HPPTARGET_HPP_

#include <vector>
#include "FileSystem.hpp"

class HppTarget
{
public:
    void include(const FileSystem::PathType& file_contents)
    {
        includes_.push_back(file_contents);
    }

    void add_subscriber_type(FileSystem::FileContentType&& subscriber_type)
    {
//        subscriber_types_.push_back(subscriber_type.substr(0, subscriber_type.size() - 1));
        subscriber_types_.push_back(std::move(subscriber_type));
    }

    FileSystem::FileContentType to_file_content() const
    {
        const FileSystem::FileContentType namespace_name = "DI";
        const FileSystem::FileContentType build_di_container_fn_begin =
            "\nDI_CONSTEXPR auto build_di_container()\n"
            "{\n"
            "\tstd::tuple concatenated_configs = std::tuple_cat(\n";
        FileSystem::FileContentType file_content = "#ifndef DI_HPP_\n"
                                                   "#define DI_HPP_\n"
                                                   "\n"
                                                   "#include \"fast_di/udil/FastDI.hpp\"\n\n";

        for (const auto& include_path: includes_)
        {
            file_content +=  "#include \"" + include_path.string() + "\"\n";
        }

        file_content += build_di_container_fn_begin;

        for (std::size_t i = 0u; i < subscriber_types_.size(); ++i)
        {
            file_content += "\t\t" + namespace_name + "::DI_SUBSCRIBER("
                + subscriber_types_[i]
                + (i != subscriber_types_.size() - 1 ? "),\n" : ")\n");
        }

        file_content += "\t);\n"
                        "\n"
                        "\treturn fast_di::udil::form_di_container(concatenated_configs);\n"
                        "}\n"
                        "\n"
                        "#endif //DI_HPP_";

        return file_content;
    }

private:
    std::vector<FileSystem::PathType> includes_;
    std::vector<FileSystem::FileContentType> subscriber_types_;
};

#endif //HPPTARGET_HPP_

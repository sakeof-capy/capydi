#ifndef CAPYGATOR_CAPYDI_TARGET_HPP_
#define CAPYGATOR_CAPYDI_TARGET_HPP_

#include "FileSystem.hpp"

#include <vector>
#include <format>
#include <ranges>
#include <algorithm>
#include <string>

class CapydiTarget
{
public:
    void include(const FileSystem::PathType& file_contents)
    {
        includes_.push_back(file_contents);
    }

    FileSystem::FileContentType to_file_content() const
    {
        FileSystem::FileContentType includes_section{};
        std::vector<FileSystem::FileContentType> config_identifiers{};

        includes_section += 
            "#define STORE_GENERATE_UNIQUE_NAME GENERATE_UNIQUE_NAME\n";

        for (const FileSystem::PathType& include_path: this->includes_)
        {
            FileSystem::FileContentType 
                config_identifier = as_config_identifier(include_path);

            includes_section += std::format(
                "#undef GENERATE_UNIQUE_NAME\n"
                "#define GENERATE_UNIQUE_NAME {}\n"
                "#include \"{}\"\n\n",
                config_identifier,
                include_path.string()
            );
            // file_content += "#include \"" + include_path.string() + "\"\n";

            config_identifiers.push_back(std::move(config_identifier));
        }

        includes_section += 
            "#undef GENERATE_UNIQUE_NAME\n"
            "#define GENERATE_UNIQUE_NAME STORE_GENERATE_UNIQUE_NAME\n"
            "#undef STORE_GENERATE_UNIQUE_NAME\n";

        const FileSystem::FileContentType config_identifiers_string = config_identifiers
            | std::views::join_with(FileSystem::FileContentType(",\n        "))
            | std::ranges::to<FileSystem::FileContentType>();

        const FileSystem::FileContentType container_builder_code = std::format(
            "namespace capy::di\n"
            "{{\n"
            "\n"
            "[[nodiscard]] inline auto build_up_container()\n"
            "{{\n"
            "    auto configs_tuple = std::tuple_cat(\n"
            "        {}\n"
            "    );\n"
            "\n"
            "    return std::apply([]<typename... Configs>(Configs&&... configs) {{\n"
            "        return capy::di::DI {{\n"
            "            std::forward<Configs>(configs)...\n"
            "        }};\n"
            "    }}, std::move(configs_tuple));\n"
            "}}\n"
            "\n"
            "}}\n",
            config_identifiers_string
        );

        return std::format(
            "{}\n{}\n{}\n{}\n{}", 
            HPP_INCLUDE_GUARD_BEGIN,
            HPP_DEFAULT_INCLUDES,
            includes_section,
            container_builder_code,
            HPP_INCLUDE_GUARD_END
        );
    }

    static FileSystem::FileContentType as_config_identifier(
        const FileSystem::PathType& path
    ) {
        FileSystem::FileContentType path_string = path.string();
        
        std::ranges::replace_if(path_string, [](auto character) {
            return !std::isalnum(static_cast<unsigned char>(character));
        }, '_');

        return "CAPYDI_CONFIG_" + path_string;
    }

private:
    static constexpr std::string_view HPP_INCLUDE_GUARD_BEGIN 
        =   "#ifndef CAPYDI_DI_AGGREGATED_HPP_\n"
            "#define CAPYDI_DI_AGGREGATED_HPP_\n";

    static constexpr std::string_view HPP_DEFAULT_INCLUDES
        =   "#include <capydi/Container.hpp>\n"
            "#include <tuple>\n";

    static constexpr std::string_view HPP_INCLUDE_GUARD_END
        =   "#endif // !CAPYDI_DI_AGGREGATED_HPP_\n";

private:
    std::vector<FileSystem::PathType> includes_;
};

#endif // !CAPYGATOR_CAPYDI_TARGET_HPP_

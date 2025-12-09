#ifndef CONFIG_TYPE_HPP_
#define CONFIG_TYPE_HPP_

namespace capy::di
{
    
enum class ConfigType : unsigned char
{
    CREATIONAL,
    DECORATIVE,
    CHAINABLE,

    SIZE
};

}

#endif // !CONFIG_TYPE_HPP_

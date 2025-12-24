#ifndef UNIFY_TEMPLATE_HPP_
#define UNIFY_TEMPLATE_HPP_

#include "capymeta/concepts/MetaCallable.hpp"

#include <array>

namespace capy::meta
{                                                 
                        
template<
    template<typename...> typename Template
>              
struct template_ft                          
{                                                     
public:                                               
    static constexpr std::array META_CALLABLE_TAGS = {
        MetaCallableTag::TYPE_CALLABLE,               
    };                                                
                        
public:                                               
    template<typename... Args>                        
    struct Functor                                    
    {                                                 
        using type = typename Template<Args...>::type;
    };                                                
};                                                    
                        
template<
    template<typename...> typename Template
>              
struct template_fv                          
{                                                     
public:                                               
    static constexpr std::array META_CALLABLE_TAGS = {
        MetaCallableTag::TYPE_CALLABLE,               
        MetaCallableTag::VALUE_CALLABLE,              
    };                                                
                        
public:                                               
    template<typename... Args>                        
    struct Functor                                    
    {                                                 
    public:                                           
        static constexpr auto                         
            value = Template<Args...>::value;         
                        
    public:                                           
        using type = decltype(value);                 
    };                                                
};

}

#endif // !UNIFY_TEMPLATE_HPP_

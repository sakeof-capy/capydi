#ifndef DEUNIFY_TEMPLATE_HPP_
#define DEUNIFY_TEMPLATE_HPP_

#include "capymeta/primitives/MetaArity.hpp"

namespace capy::meta::implementation_details_
{

template<
    MetaArity ARITY,
    template<typename...> typename Template
>
struct FixTemplateArityTyped;

template<
    MetaArity ARITY,
    template<typename...> typename Template
>
struct FixTemplateArityValued;

}

#define PACK(...) __VA_ARGS__

#define DEUNIFY_TEMPLATE(MetaArity, TemplateType, ExpandedArgs) \
    template<                                                   \
        template<typename...> typename Template                 \
    >                                                           \
    struct FixTemplateArityTyped<MetaArity, Template>           \
    {                                                           \
        TemplateType                                            \
        struct Functor                                          \
        {                                                       \
            using type = typename Template<ExpandedArgs>::type; \
        };                                                      \
    };                                                          \
                                                                \
    template<                                                   \
        template<typename...> typename Template                 \
    >                                                           \
    struct FixTemplateArityValued<MetaArity, Template>          \
    {                                                           \
        TemplateType                                            \
        struct Functor                                          \
        {                                                       \
            static constexpr auto value                         \
                = Template<ExpandedArgs>::value;                \
        };                                                      \
    }


namespace capy::meta
{

namespace implementation_details_
{

DEUNIFY_TEMPLATE(MetaArity::N1,  PACK(template<class T>), PACK(T));
DEUNIFY_TEMPLATE(MetaArity::N2,  PACK(template<class T1, class T2>), PACK(T1, T2));
DEUNIFY_TEMPLATE(MetaArity::N3,  PACK(template<class T1, class T2, class T3>), PACK(T1, T2, T3));
DEUNIFY_TEMPLATE(MetaArity::N4,  PACK(template<class T1, class T2, class T3, class T4>), PACK(T1, T2, T3, T4));
DEUNIFY_TEMPLATE(MetaArity::N5,  PACK(template<class T1, class T2, class T3, class T4, class T5>), PACK(T1, T2, T3, T4, T5));
DEUNIFY_TEMPLATE(MetaArity::N6,  PACK(template<class T1, class T2, class T3, class T4, class T5, class T6>), PACK(T1, T2, T3, T4, T5, T6));
DEUNIFY_TEMPLATE(MetaArity::N7,  PACK(template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>), PACK(T1, T2, T3, T4, T5, T6, T7));
DEUNIFY_TEMPLATE(MetaArity::N8,  PACK(template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>), PACK(T1, T2, T3, T4, T5, T6, T8));
DEUNIFY_TEMPLATE(MetaArity::N9,  PACK(template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>), PACK(T1, T2, T3, T4, T5, T6, T8, T9));
DEUNIFY_TEMPLATE(MetaArity::N10, PACK(template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>), PACK(T1, T2, T3, T4, T5, T6, T8, T9, T10));
DEUNIFY_TEMPLATE(MetaArity::N11, PACK(template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>), PACK(T1, T2, T3, T4, T5, T6, T8, T9, T10, T11));
DEUNIFY_TEMPLATE(MetaArity::N12, PACK(template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12>), PACK(T1, T2, T3, T4, T5, T6, T8, T9, T10, T11, T12));
DEUNIFY_TEMPLATE(MetaArity::N13, PACK(template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13>), PACK(T1, T2, T3, T4, T5, T6, T8, T9, T10, T11, T12, T13));
DEUNIFY_TEMPLATE(MetaArity::N14, PACK(template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14>), PACK(T1, T2, T3, T4, T5, T6, T8, T9, T10, T11, T12, T13, T14));
DEUNIFY_TEMPLATE(MetaArity::N15, PACK(template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>), PACK(T1, T2, T3, T4, T5, T6, T8, T9, T10, T11, T12, T13, T14, T15));
DEUNIFY_TEMPLATE(MetaArity::N16, PACK(template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16>), PACK(T1, T2, T3, T4, T5, T6, T8, T9, T10, T11, T12, T13, T14, T15, T16));

}

template<
    MetaArity ARITY, 
    template<typename...> typename Template
>
using fix_template_arity_ft = typename implementation_details_::FixTemplateArityTyped<ARITY, Template>;

template<
    MetaArity ARITY, 
    template<typename...> typename Template
>
using fix_template_arity_fv = typename implementation_details_::FixTemplateArityValued<ARITY, Template>;

}

#undef PACK
#undef DEUNIFY_TEMPLATE

#endif //!DEUNIFY_TEMPLATE_HPP_

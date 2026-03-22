#ifndef CAPYDI_RESOLUTION_CONTEXT_HPP_
#define CAPYDI_RESOLUTION_CONTEXT_HPP_

namespace capy::di
{

struct ResolutionFlags
{
    bool just_created = false;
};

template<
    typename InputTuple,
    typename Container
>
struct ResolutionContext
{
    InputTuple input;
    Container& container;
    ResolutionFlags flags;
};
    
}

#endif // !CAPYDI_RESOLUTION_CONTEXT_HPP_

#ifndef SELF_TYPE_HPP_
#define SELF_TYPE_HPP_

namespace capy::di
{

template<typename T>
concept SelfTypeOwner = requires() {
    typename T::SelfType;
};

template<SelfTypeOwner T> 
using self_t = typename T::SelfType;

}

#endif // !SELF_TYPE_HPP_
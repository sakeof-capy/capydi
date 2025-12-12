#ifndef MIXED_SPINE_LEAF4_HPP_
#define MIXED_SPINE_LEAF4_HPP_

#include "SpineLeaf3.hpp"
#include "ConstSpineLeaf3.hpp"

namespace capy::di::mixed_spine_leaf_4
{

class TopTopologiesConnector
{
public:
    static constexpr int IDENTIFIER = 
        spine_leaf_3::RootSpine::IDENTIFIER + 
        const_spine_leaf_3::RootSpine::IDENTIFIER;

public:
    TopTopologiesConnector(
        spine_leaf_3::RootSpine& root_spine,
        const const_spine_leaf_3::RootSpine& const_root_spine
    )
        : root_spine_ { root_spine }
        , const_root_spine_ { const_root_spine }
    {}

public:
    static TopTopologiesConnector create(
        spine_leaf_3::RootSpine& root_spine,
        const const_spine_leaf_3::RootSpine& const_root_spine
    )
    {
        return TopTopologiesConnector { root_spine, const_root_spine };
    }

public:
    int cross_sum()
    {
        return this->root_spine_.sum() 
             + this->const_root_spine_.sum();
    }

private:
    spine_leaf_3::RootSpine& root_spine_;
    const const_spine_leaf_3::RootSpine& const_root_spine_;
};

};

#endif // !MIXED_SPINE_LEAF4_HPP_

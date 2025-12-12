#ifndef CONST_SPINE_LEAF_3_HPP_
#define CONST_SPINE_LEAF_3_HPP_

namespace capy::di::const_spine_leaf_3
{

class Leaf1
{
public:
    static constexpr int IDENTIFIER = 123;

public:
    constexpr explicit Leaf1()
        : value_ { IDENTIFIER }
    {}

    constexpr static Leaf1 create()
    {
        return Leaf1{};
    }

    constexpr int get() const
    {
        return value_;
    }

private:
    int value_;
};

class Leaf2
{
public:
    static constexpr int IDENTIFIER = 321;

public:
    constexpr explicit Leaf2()
        : value_ { IDENTIFIER }
    {}

    constexpr static Leaf2 create()
    {
        return Leaf2{};
    }

    constexpr int get() const
    {
        return value_;
    }

private:
    int value_;
};

class Spine1
{
public:
    static constexpr int IDENTIFIER = 
        Leaf1::IDENTIFIER + Leaf2::IDENTIFIER;

public:
    constexpr Spine1(const Leaf1& l1, const Leaf2& l2)
        : leaf1_ { l1 }, leaf2_ { l2 }
    {}

    constexpr static Spine1 create(const Leaf1& l1, const Leaf2& l2)
    {
        return Spine1 { l1, l2 };
    }

    constexpr int sum() const
    {
        return leaf1_.get() + leaf2_.get();
    }

private:
    const Leaf1& leaf1_;
    const Leaf2& leaf2_;
};

class Spine2
{
public:
    static constexpr int IDENTIFIER = 
        Leaf1::IDENTIFIER + Leaf2::IDENTIFIER;

public:
    constexpr Spine2(const Leaf1& l1, const Leaf2& l2)
        : leaf1_ { l1 }, leaf2_ { l2 }
    {}

    constexpr static Spine2 create(const Leaf1& l1, const Leaf2& l2)
    {
        return Spine2 { l1, l2 };
    }

    constexpr int sum() const
    {
        return leaf1_.get() + leaf2_.get();
    }

private:
    const Leaf1& leaf1_;
    const Leaf2& leaf2_;
};

class RootSpine
{
public:
    static constexpr int IDENTIFIER = 
        Spine1::IDENTIFIER + Spine2::IDENTIFIER;

public:
    constexpr RootSpine(const Spine1& s1, const Spine2& s2)
        : spine1_ { s1 }, spine2_ { s2 }
    {}

    constexpr static RootSpine create(const Spine1& s1, const Spine2& s2)
    {
        return RootSpine { s1, s2 };
    }

    constexpr int sum() const
    {
        return spine1_.sum() + spine2_.sum();
    }

private:
    const Spine1& spine1_;
    const Spine2& spine2_;
};

}

#endif // !CONST_SPINE_LEAF_3_HPP_

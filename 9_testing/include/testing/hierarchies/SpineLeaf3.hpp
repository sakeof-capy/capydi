#ifndef SPINE_LEAF_3_HPP_
#define SPINE_LEAF_3_HPP_

namespace capy::di::spine_leaf_3
{

class Leaf1
{
public:
    static constexpr int IDENTIFIER = 123;

public:
    explicit Leaf1()
        : value_ { IDENTIFIER }
    {}

    static Leaf1 create()
    {
        return Leaf1{};
    }

    int get() const
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
    explicit Leaf2()
        : value_ { IDENTIFIER }
    {}

    static Leaf2 create()
    {
        return Leaf2{};
    }

    int get() const
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
    Spine1(const Leaf1& l1, Leaf2& l2)
        : leaf1_ { l1 }, leaf2_ { l2 }
    {}

    static Spine1 create(const Leaf1& l1, Leaf2& l2)
    {
        return Spine1 { l1, l2 };
    }

    int sum() const
    {
        return leaf1_.get() + leaf2_.get();
    }

private:
    const Leaf1& leaf1_;
    Leaf2& leaf2_;
};

class Spine2
{
public:
    static constexpr int IDENTIFIER = 
        Leaf1::IDENTIFIER + Leaf2::IDENTIFIER;

public:
    Spine2(const Leaf1& l1, Leaf2& l2)
        : leaf1_ { l1 }, leaf2_ { l2 }
    {}

    static Spine2 create(const Leaf1& l1, Leaf2& l2)
    {
        return Spine2 { l1, l2 };
    }

    int sum() const
    {
        return leaf1_.get() + leaf2_.get();
    }

private:
    const Leaf1& leaf1_;
    Leaf2& leaf2_;
};

class RootSpine
{
public:
    static constexpr int IDENTIFIER = 
        Spine1::IDENTIFIER + Spine2::IDENTIFIER;

public:
    RootSpine(const Spine1& s1, Spine2& s2)
        : spine1_ { s1 }, spine2_ { s2 }
    {}

    static RootSpine create(const Spine1& s1, Spine2& s2)
    {
        return RootSpine { s1, s2 };
    }

    int sum() const
    {
        return spine1_.sum() + spine2_.sum();
    }

private:
    const Spine1& spine1_;
    Spine2& spine2_;
};

}

#endif // !SPINE_LEAF_3_HPP_

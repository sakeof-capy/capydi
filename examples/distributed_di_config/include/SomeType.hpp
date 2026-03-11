#ifndef SOME_TYPE_HPP_
#define SOME_TYPE_HPP_

struct SomeType
{
    int a = 32;
    
    inline static SomeType create()
    {
        return {};
    }
};


struct SomeOtherType
{
    SomeType& some_type;
    
    inline static SomeOtherType create(SomeType& some)
    {
        return SomeOtherType { .some_type = some };
    }

    int operation() const
    {
        return this->some_type.a + 64;
    }
};

#endif // !SOME_TYPE_HPP_

#ifndef CAPYDI_OPERATIONS_HPP_
#define CAPYDI_OPERATIONS_HPP_


namespace capy::di::operations
{


class IValue
{
public:
    virtual ~IValue() = default;

public:
    virtual int get_value() = 0;
    virtual const char* get_name() const = 0;
};

class Value1 : public IValue
{
public:
    static constexpr int VALUE = 1;

public:
    int get_value() override
    {
        return VALUE;
    }

    const char* get_name() const override
    {
        return "value1_name";
    }

public:
    static Value1 create()
    {
        return Value1{};
    }
};

class Value3 : public IValue
{
public:
    static constexpr int VALUE = 3;

public:
    int get_value() override
    {
        return VALUE;
    }

    const char* get_name() const override
    {
        return "value3_name";
    }

public:
    static Value3 create()
    {
        return Value3{};
    }
};

class MultiplyBy2_Decorator : public IValue
{
public:
    explicit MultiplyBy2_Decorator(IValue& decoratee)
        : decoratee_ { decoratee }
    {}

public:
    int get_value() override
    {
        return 2 * decoratee_.get_value();
    }

    const char* get_name() const override
    {
        return decoratee_.get_name();
    }

public:
    static MultiplyBy2_Decorator create(IValue& decoratee)
    {
        return MultiplyBy2_Decorator { decoratee };
    }

private:
    IValue& decoratee_;
};

class Plus
{
public:
    Plus(IValue& value1, IValue& value2)
        : value1_ { value1 }
        , value2_ { value2 }
    {}

public:
    int evaluate()
    {
        return value1_.get_value() + value2_.get_value();
    }

    static Plus create(IValue& value1, IValue& value2)
    {
        return Plus { value1, value2 };
    }

private:
    IValue& value1_;
    IValue& value2_;
};

}


#endif // !CAPYDI_OPERATIONS_HPP_

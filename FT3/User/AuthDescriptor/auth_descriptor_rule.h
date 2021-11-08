#include <vector>
#include "../../../src/GTV/abstract_value_factory.h"
#include "../../../src/GTV/array_value.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {

class IAuthdescriptorRule
{
public:
	std::shared_ptr<ArrayValue> ToGTV();
};

class Rules
{
public:
	static RuleVariable BlockHeight()
    {
        return RuleVariable("block_height");
    }

    static RuleVariable BlockTime()
    {
        return RuleVariable("block_time");
    }

    static RuleVariable OperationCount()
    {
        return RuleVariable("op_count");
    }
};

class RuleVariable
{
public:
	RuleVariable(std::string variable)
    {
        this->variable_ = variable;
    }

    RuleExpression LessThan(long value)
    {
        return this->Expression("lt", value);
    }

	RuleExpression LessOrEqual(long value)
    {
        return this->Expression("le", value);
    }

    RuleExpression Equal(long value)
    {
        return this->Expression("eq", value);
    }

    RuleExpression GreaterThan(long value)
    {
        return this->Expression("gt", value);
    }

    RuleExpression GreaterOrEqual(long value)
    {
        return this->Expression("ge", value);
    }

private:
	std::string variable_;

	RuleExpression Expression(std::string op, long value)
	{
		return RuleExpression(this->variable_, op, value);
	}
};

class RuleExpression : IAuthdescriptorRule
{
public:
	std::string name_;
    std::string operator_;
	long value_;

	RuleExpression() {};

    RuleExpression(std::string name, std::string op, long value)
    {
        name = name;
        operator_ = op;
        value = value;
    }

    RuleCompositeExpressionOperator And()
    {
        return RuleCompositeExpressionOperator(*this, "and");
    }

	std::shared_ptr<ArrayValue> ToGTV()
    {
		std::shared_ptr<ArrayValue> gtv = AbstractValueFactory::EmptyArray();
		gtv->Add(AbstractValueFactory::Build(this->name_));
		gtv->Add(AbstractValueFactory::Build(this->operator_));
		gtv->Add(AbstractValueFactory::Build(this->value_));
        return gtv;
    }
};

class RuleCompositeExpressionOperator
{
public:
	IAuthdescriptorRule expression_;
    std::string operator_;

    RuleCompositeExpressionOperator(IAuthdescriptorRule expression, std::string op)
    {
        expression_ = expression;
        operator_ = op;
    }

    RuleCompositeExpressionVariable BlockHeight()
    {
        return RuleCompositeExpressionVariable(this->expression_, "block_height", this->operator_);
    }

    RuleCompositeExpressionVariable BlockTime()
    {
        return RuleCompositeExpressionVariable(this->expression_, "block_time", this->operator_);
    }

    RuleCompositeExpressionVariable OperationCount()
    {
        return RuleCompositeExpressionVariable(this->expression_, "op_count", this->operator_);
    }
};

class RuleCompositeExpressionVariable
{
public:
	IAuthdescriptorRule expression_;
    std::string name_;
    std::string operator_;

    RuleCompositeExpressionVariable(IAuthdescriptorRule expression, std::string name, std::string op)
    {
        expression_ = expression;
        name_ = name;
        operator_ = op;
    }

    RuleCompositeExpression LessThan(long value)
    {
        this->CompositeExpression("lt", value);
    }

    RuleCompositeExpression LessOrEqual(long value)
    {
        this->CompositeExpression("le", value);
    }

    RuleCompositeExpression Equal(long value)
    {
        this->CompositeExpression("eq", value);
    }

    RuleCompositeExpression GreaterThan(long value)
    {
        this->CompositeExpression("gt", value);
    }

    RuleCompositeExpression GreaterOrEqual(long value)
    {
        return CompositeExpression("ge", value);
    }

    RuleCompositeExpression CompositeExpression(std::string op, long value)
    {
        return RuleCompositeExpression(this->operator_, this->expression_, RuleExpression(this->name_, op, value));
    }
};

class RuleCompositeExpression : IAuthdescriptorRule
{
public:
	std::string operator_;
    IAuthdescriptorRule left_;
    RuleExpression right_;

    RuleCompositeExpression(std::string op, IAuthdescriptorRule left, RuleExpression right)
    {
        operator_ = op;
        left_ = left;
        right_ = right;
    }

    RuleCompositeExpressionOperator And()
    {
        return RuleCompositeExpressionOperator(*this, "and");
    }

	std::shared_ptr<AbstractValue> ToGTV()
    {
		std::shared_ptr<ArrayValue> gtv = AbstractValueFactory::EmptyArray();
		std::shared_ptr<ArrayValue> left_gtv = left_.ToGTV();
		std::shared_ptr<ArrayValue> right_gtv = right_.ToGTV();
		
		gtv->Add(left_gtv);
		gtv->Add(AbstractValueFactory::Build(this->operator_));
		gtv->Add(right_gtv);
		
		return gtv;
	}
};
} // namespace http
} // namespace postchain
} // namespace chromia
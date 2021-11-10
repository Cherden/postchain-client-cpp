#include "auth_descriptor_rule.h"

namespace chromia {
namespace postchain {
namespace ft3 {
	
RuleVariable Rules::BlockHeight()
{
	return RuleVariable("block_height");
}

RuleVariable Rules::BlockTime()
{
	return RuleVariable("block_time");
}

RuleVariable Rules::OperationCount()
{
	return RuleVariable("op_count");
}

//-----------------------------------------------------------------------------------

RuleVariable::RuleVariable(std::string variable)
{
	this->variable_ = variable;
}

RuleExpression RuleVariable::LessThan(long value)
{
	return this->Expression("lt", value);
}

RuleExpression RuleVariable::LessOrEqual(long value)
{
	return this->Expression("le", value);
}

RuleExpression RuleVariable::Equal(long value)
{
	return this->Expression("eq", value);
}

RuleExpression RuleVariable::GreaterThan(long value)
{
	return this->Expression("gt", value);
}

RuleExpression RuleVariable::GreaterOrEqual(long value)
{
	return this->Expression("ge", value);
}

RuleExpression RuleVariable::Expression(std::string op, long value)
{
	return RuleExpression(this->variable_, op, value);
}

//-----------------------------------------------------------------------------------

RuleExpression::RuleExpression() {}

RuleExpression::RuleExpression(std::string name, std::string op, long value)
{
	name = name;
	operator_ = op;
	value = value;
}

RuleCompositeExpressionOperator RuleExpression::And()
{
	std::shared_ptr<RuleExpression> this_shared(this);
	return RuleCompositeExpressionOperator(std::dynamic_pointer_cast<IAuthdescriptorRule>(this_shared), "and");
}

std::shared_ptr<ArrayValue> RuleExpression::ToGTV()
{
	std::shared_ptr<ArrayValue> gtv = AbstractValueFactory::EmptyArray();
	gtv->Add(AbstractValueFactory::Build(this->name_));
	gtv->Add(AbstractValueFactory::Build(this->operator_));
	gtv->Add(AbstractValueFactory::Build(this->value_));
	return gtv;
}

//-----------------------------------------------------------------------------------

RuleCompositeExpressionOperator::RuleCompositeExpressionOperator(std::shared_ptr<IAuthdescriptorRule> expression, std::string op)
{
	expression_ = expression;
	operator_ = op;
}

RuleCompositeExpressionVariable RuleCompositeExpressionOperator::BlockHeight()
{
	return RuleCompositeExpressionVariable(this->expression_, "block_height", this->operator_);
}

RuleCompositeExpressionVariable RuleCompositeExpressionOperator::BlockTime()
{
	return RuleCompositeExpressionVariable(this->expression_, "block_time", this->operator_);
}

RuleCompositeExpressionVariable RuleCompositeExpressionOperator::OperationCount()
{
	return RuleCompositeExpressionVariable(this->expression_, "op_count", this->operator_);
}

//-----------------------------------------------------------------------------------

RuleCompositeExpressionVariable::RuleCompositeExpressionVariable(std::shared_ptr<IAuthdescriptorRule> expression, std::string name, std::string op)
{
	expression_ = expression;
	name_ = name;
	operator_ = op;
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::LessThan(long value)
{
	return this->CompositeExpression("lt", value);
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::LessOrEqual(long value)
{
	return this->CompositeExpression("le", value);
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::Equal(long value)
{
	return this->CompositeExpression("eq", value);
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::GreaterThan(long value)
{
	return this->CompositeExpression("gt", value);
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::GreaterOrEqual(long value)
{
	return CompositeExpression("ge", value);
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::CompositeExpression(std::string op, long value)
{
	return std::make_shared<RuleCompositeExpression>(this->operator_, this->expression_, std::make_shared<RuleExpression>(this->name_, op, value));
}

//-----------------------------------------------------------------------------------

RuleCompositeExpression::RuleCompositeExpression(std::string op, std::shared_ptr<IAuthdescriptorRule> left, std::shared_ptr<RuleExpression> right)
{
	operator_ = op;
	left_ = left;
	right_ = right;
}

RuleCompositeExpressionOperator RuleCompositeExpression::And()
{
	std::shared_ptr<RuleCompositeExpression> this_shared(this);
	return RuleCompositeExpressionOperator(std::dynamic_pointer_cast<IAuthdescriptorRule>(this_shared), "and");
}

std::shared_ptr<ArrayValue> RuleCompositeExpression::ToGTV()
{
	std::shared_ptr<ArrayValue> gtv = AbstractValueFactory::EmptyArray();
	std::shared_ptr<ArrayValue> left_gtv = left_->ToGTV();
	std::shared_ptr<ArrayValue> right_gtv = right_->ToGTV();

	gtv->Add(left_gtv);
	gtv->Add(AbstractValueFactory::Build(this->operator_));
	gtv->Add(right_gtv);

	return gtv;
}

} // namespace http
} // namespace postchain
} // namespace chromia

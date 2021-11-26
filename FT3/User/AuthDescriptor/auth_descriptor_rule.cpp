#include "auth_descriptor_rule.h"

namespace chromia {
namespace postchain {
namespace ft3 {
	
std::shared_ptr<RuleVariable> Rules::BlockHeight()
{
	return std::make_shared<RuleVariable>("block_height");
}

std::shared_ptr<RuleVariable> Rules::BlockTime()
{
	return std::make_shared<RuleVariable>("block_time");
}

std::shared_ptr<RuleVariable> Rules::OperationCount()
{
	return std::make_shared<RuleVariable>("op_count");
}

//-----------------------------------------------------------------------------------

RuleVariable::RuleVariable(std::string variable)
{
	this->variable_ = variable;
}

std::shared_ptr<RuleExpression> RuleVariable::LessThan(long long value)
{
	return this->Expression("lt", value);
}

std::shared_ptr<RuleExpression> RuleVariable::LessOrEqual(long long value)
{
	return this->Expression("le", value);
}

std::shared_ptr<RuleExpression> RuleVariable::Equal(long long value)
{
	return this->Expression("eq", value);
}

std::shared_ptr<RuleExpression> RuleVariable::GreaterThan(long long value)
{
	return this->Expression("gt", value);
}

std::shared_ptr<RuleExpression> RuleVariable::GreaterOrEqual(long long value)
{
	return this->Expression("ge", value);
}

std::shared_ptr<RuleExpression> RuleVariable::Expression(std::string op, long long value)
{
	return std::make_shared<RuleExpression>(this->variable_, op, value);
}

//-----------------------------------------------------------------------------------

RuleExpression::RuleExpression() {}

RuleExpression::RuleExpression(std::string name, std::string op, long long value)
{
	name_ = name;
	operator_ = op;
	value_ = value;
}

std::shared_ptr<RuleCompositeExpressionOperator> RuleExpression::And()
{
	return std::make_shared<RuleCompositeExpressionOperator>(shared_from_this(), "and");
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

std::shared_ptr<RuleCompositeExpressionVariable> RuleCompositeExpressionOperator::BlockHeight()
{
	return std::make_shared<RuleCompositeExpressionVariable>(this->expression_, "block_height", this->operator_);
}

std::shared_ptr<RuleCompositeExpressionVariable> RuleCompositeExpressionOperator::BlockTime()
{
	return std::make_shared<RuleCompositeExpressionVariable>(this->expression_, "block_time", this->operator_);
}

std::shared_ptr<RuleCompositeExpressionVariable> RuleCompositeExpressionOperator::OperationCount()
{
	return std::make_shared<RuleCompositeExpressionVariable>(this->expression_, "op_count", this->operator_);
}

//-----------------------------------------------------------------------------------

RuleCompositeExpressionVariable::RuleCompositeExpressionVariable(std::shared_ptr<IAuthdescriptorRule> expression, std::string name, std::string op)
{
	expression_ = expression;
	name_ = name;
	operator_ = op;
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::LessThan(long long value)
{
	return this->CompositeExpression("lt", value);
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::LessOrEqual(long long value)
{
	return this->CompositeExpression("le", value);
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::Equal(long long value)
{
	return this->CompositeExpression("eq", value);
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::GreaterThan(long long value)
{
	return this->CompositeExpression("gt", value);
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::GreaterOrEqual(long long value)
{
	return this->CompositeExpression("ge", value);
}

std::shared_ptr<RuleCompositeExpression> RuleCompositeExpressionVariable::CompositeExpression(std::string op, long long value)
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

std::shared_ptr<RuleCompositeExpressionOperator> RuleCompositeExpression::And()
{
	return std::make_shared<RuleCompositeExpressionOperator>(shared_from_this(), "and");
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

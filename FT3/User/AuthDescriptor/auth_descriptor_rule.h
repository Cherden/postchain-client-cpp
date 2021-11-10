#pragma once

#include <vector>
#include "../../../src/GTV/abstract_value_factory.h"
#include "../../../src/GTV/array_value.h"
#include "../../forward_declarations.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {

class Rules
{
public:
	static RuleVariable BlockHeight();

	static RuleVariable BlockTime();

	static RuleVariable OperationCount();
};

class RuleVariable
{
public:
	RuleVariable(std::string variable);

	RuleExpression LessThan(long value);

	RuleExpression LessOrEqual(long value);

	RuleExpression Equal(long value);

	RuleExpression GreaterThan(long value);

	RuleExpression GreaterOrEqual(long value);

private:
	std::string variable_;

	RuleExpression Expression(std::string op, long value);
};

class RuleExpression : IAuthdescriptorRule
{
public:
	std::string name_;
    std::string operator_;
	long value_;

	RuleExpression();

	RuleExpression(std::string name, std::string op, long value);

	RuleCompositeExpressionOperator And();

	std::shared_ptr<ArrayValue> ToGTV();
};

class RuleCompositeExpressionOperator
{
public:
	std::shared_ptr<IAuthdescriptorRule> expression_;
    std::string operator_;

	RuleCompositeExpressionOperator(std::shared_ptr<IAuthdescriptorRule> expression, std::string op);

	RuleCompositeExpressionVariable BlockHeight();

	RuleCompositeExpressionVariable BlockTime();

	RuleCompositeExpressionVariable OperationCount();
};

class RuleCompositeExpressionVariable
{
public:
	std::shared_ptr<IAuthdescriptorRule> expression_;
    std::string name_;
    std::string operator_;

	RuleCompositeExpressionVariable(std::shared_ptr<IAuthdescriptorRule> expression, std::string name, std::string op);

	std::shared_ptr<RuleCompositeExpression> LessThan(long value);

	std::shared_ptr<RuleCompositeExpression> LessOrEqual(long value);

	std::shared_ptr<RuleCompositeExpression> Equal(long value);

	std::shared_ptr<RuleCompositeExpression> GreaterThan(long value);

	std::shared_ptr<RuleCompositeExpression> GreaterOrEqual(long value);

	std::shared_ptr<RuleCompositeExpression> CompositeExpression(std::string op, long value);
};

class RuleCompositeExpression : IAuthdescriptorRule
{
public:
	std::string operator_;
	std::shared_ptr<IAuthdescriptorRule> left_;
	std::shared_ptr<RuleExpression> right_;

	RuleCompositeExpression(std::string op, std::shared_ptr<IAuthdescriptorRule> left, std::shared_ptr<RuleExpression> right);

	RuleCompositeExpressionOperator And();

	std::shared_ptr<ArrayValue> ToGTV();

};
} // namespace http
} // namespace postchain
} // namespace chromia
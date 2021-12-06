#pragma once

#include <vector>
#include "../../../PostchainClient/GTV/abstract_value_factory.h"
#include "../../../PostchainClient/GTV/array_value.h"
#include "../../forward_declarations.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {

class Rules
{
public:
	static std::shared_ptr<RuleVariable> BlockHeight();

	static std::shared_ptr<RuleVariable> BlockTime();

	static std::shared_ptr<RuleVariable> OperationCount();
};

class RuleVariable
{
public:
	RuleVariable(std::string variable);

	std::shared_ptr<RuleExpression> LessThan(long long value);

	std::shared_ptr<RuleExpression> LessOrEqual(long long value);

	std::shared_ptr<RuleExpression> Equal(long long value);

	std::shared_ptr<RuleExpression> GreaterThan(long long value);

	std::shared_ptr<RuleExpression> GreaterOrEqual(long long value);

private:
	std::string variable_;

	std::shared_ptr<RuleExpression> Expression(std::string op, long long value);
};

class RuleExpression : public IAuthdescriptorRule
{
public:
	std::string name_;
    std::string operator_;
	long long value_;

	RuleExpression();

	RuleExpression(std::string name, std::string op, long long value);

	std::shared_ptr<RuleCompositeExpressionOperator> And();

	std::shared_ptr<ArrayValue> ToGTV();
};

class RuleCompositeExpressionOperator
{
public:
	std::shared_ptr<IAuthdescriptorRule> expression_;
    std::string operator_;

	RuleCompositeExpressionOperator(std::shared_ptr<IAuthdescriptorRule> expression, std::string op);

	std::shared_ptr<RuleCompositeExpressionVariable> BlockHeight();

	std::shared_ptr<RuleCompositeExpressionVariable> BlockTime();

	std::shared_ptr<RuleCompositeExpressionVariable> OperationCount();
};

class RuleCompositeExpressionVariable
{
public:
	std::shared_ptr<IAuthdescriptorRule> expression_;
    std::string name_;
    std::string operator_;

	RuleCompositeExpressionVariable(std::shared_ptr<IAuthdescriptorRule> expression, std::string name, std::string op);

	std::shared_ptr<RuleCompositeExpression> LessThan(long long value);

	std::shared_ptr<RuleCompositeExpression> LessOrEqual(long long value);

	std::shared_ptr<RuleCompositeExpression> Equal(long long value);

	std::shared_ptr<RuleCompositeExpression> GreaterThan(long long value);

	std::shared_ptr<RuleCompositeExpression> GreaterOrEqual(long long value);

	std::shared_ptr<RuleCompositeExpression> CompositeExpression(std::string op, long long value);
};

class RuleCompositeExpression : public IAuthdescriptorRule
{
public:
	std::string operator_;
	std::shared_ptr<IAuthdescriptorRule> left_;
	std::shared_ptr<RuleExpression> right_;

	RuleCompositeExpression(std::string op, std::shared_ptr<IAuthdescriptorRule> left, std::shared_ptr<RuleExpression> right);

	std::shared_ptr<RuleCompositeExpressionOperator> And();

	std::shared_ptr<ArrayValue> ToGTV();

};
} // namespace http
} // namespace postchain
} // namespace chromia
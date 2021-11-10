#pragma once

namespace chromia {
namespace postchain {
namespace ft3 {

enum AuthType;
enum FlagsType;

class Blockchain;
class BlockchainInfo;
class BlockchainSession;
class DirectoryService;
class DirectoryServiceBase;
class RateLimit;
class RateLimitInfo;

class IAuthdescriptorRule;
class TransactionBuilder;
class Transaction;
class User;
class Operation;
class Flags;
class AuthDescriptor;
class Account;
class Asset;
class AccountOperations;

class RuleVariable;
class Rules;
class IAuthdescriptorRule;
class RuleExpression;
class RuleCompositeExpressionOperator;
class RuleCompositeExpressionVariable;
class RuleCompositeExpression;
class AssetBalance;

class GtvSerializable
{
public:
	virtual std::shared_ptr<ArrayValue> ToGTV() = 0;
	virtual ~GtvSerializable() = default;
};

class IAuthdescriptorRule
{
public:
	virtual std::shared_ptr<ArrayValue> ToGTV() = 0;
	virtual ~IAuthdescriptorRule() = default;
};

} // namespace http
} // namespace postchain
} // namespace chromia
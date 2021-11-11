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

class TransactionBuilder;
class Transaction;
class User;
class Operation;
class Flags;
class Account;
class Asset;
class AccountOperations;

class RuleVariable;
class Rules;
class RuleExpression;
class RuleCompositeExpressionOperator;
class RuleCompositeExpressionVariable;
class RuleCompositeExpression;
class AssetBalance;

class SingleSignatureAuthDescriptor;
class MultiSignatureAuthDescriptor;

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

class AuthDescriptor : public GtvSerializable
{
public:
	std::string id_;
	std::vector<std::vector<byte>> signers_;
	std::shared_ptr<IAuthdescriptorRule> rule_;
	std::vector<byte> Hash() {};
};


} // namespace http
} // namespace postchain
} // namespace chromia
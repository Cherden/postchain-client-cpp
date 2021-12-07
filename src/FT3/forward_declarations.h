#pragma once

#include "../PostchainClient/GTV/array_value.h"
#include "../PostchainClient/common.h"

using namespace chromia::postchain::gtv;
using namespace chromia;

namespace chromia {
namespace postchain {
	
struct QueryObject;

namespace ft3 {

enum AuthType;
enum FlagsType;

class KeyPair;

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

class IAuthdescriptorRule : public std::enable_shared_from_this<IAuthdescriptorRule>
{
public:
	virtual std::shared_ptr<ArrayValue> ToGTV() = 0;
	virtual ~IAuthdescriptorRule() = default;
};

class AuthDescriptor : public GtvSerializable
{
public:
	virtual ~AuthDescriptor() = default;

	virtual std::string ID() = 0;
	virtual std::vector<std::vector<byte>> Signers() = 0;
	virtual std::shared_ptr<IAuthdescriptorRule> Rule() = 0;
	virtual std::vector<byte> Hash() = 0;
};


} // namespace ft3
} // namespace postchain
} // namespace chromia
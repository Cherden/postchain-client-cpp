#pragma once 

//#ifdef FT3_TESTS

#include <functional>
#include <string>
#include "../../src/common.h"
#include "../../FT3/forward_declarations.h"
#include "../../FT3/User/AuthDescriptor/auth_descriptor_rule.h"
#include "../../FT3/User/asset.h"
#include "../../src/postchain_util.h"
#include "../TestUtil/blockchain_util.h"
#include "../TestUtil/test_user.h"
#include "../TestUtil/test_util.h"
#include "../TestUtil/account_builder.h"
#include "../TestUtil/test_operators.h"

using namespace chromia::postchain::ft3;
using namespace chromia::postchain;

class AuthDescriptorRuleTest
{
private:
	std::shared_ptr<Blockchain> blockchain_ = nullptr;

	void SetupBlockchain();

	static void EmptyCallback() {}

	static void DefaultErrorHandler(std::string error);

	void AddAuthDescriptorTo(std::shared_ptr<Account> account, std::shared_ptr<User> admin_user, std::shared_ptr<User> user, std::function<void()> on_success);

	void SourceAccount(std::shared_ptr<Blockchain> blockchain, std::shared_ptr<User> user, std::shared_ptr<Asset> asset, std::function<void(std::shared_ptr<Account>)> on_success)
	{
		std::shared_ptr<AccountBuilder> builder = AccountBuilder::CreateAccountBuilder(blockchain, user);
		builder->WithBalance(asset, 200);
		builder->WithPoints(5);
		builder->Build(on_success);
	}

	void DestinationAccount(std::shared_ptr<Blockchain> blockchain, std::function<void(std::shared_ptr<Account>)> on_success)
	{
		std::shared_ptr<AccountBuilder> builder = AccountBuilder::CreateAccountBuilder(blockchain);
		builder->Build(on_success);
	}

	void CreateAsset(std::shared_ptr<Blockchain> blockchain, std::function<void(std::shared_ptr<Asset>)> on_success)
	{
		Asset::Register(
			TestUtil::GenerateAssetName(),
			TestUtil::GenerateId(),
			blockchain, on_success,
			DefaultErrorHandler
		);
	}

public:

	// should succeed when calling operations, number of times less than or equal to value set by operation count rule
	bool AuthDescriptorRuleTestRun1();

	// should fail when calling operations, number of times more than value set by operation count rule
	bool AuthDescriptorRuleTestRun2();

	// should fail when current time is greater than time defined by 'less than' block time rule
	bool AuthDescriptorRuleTestRun3();

	// should succeed when current time is less than time defined by 'less than' block time rule
	bool AuthDescriptorRuleTestRun4();

	// should succeed when current block height is less than value defined by 'less than' block height rule
	bool AuthDescriptorRuleTestRun5();

	// should fail when current block height is greater than value defined by 'less than' block height rule
	bool AuthDescriptorRuleTestRun6();

	// should fail if operation is executed before timestamp defined by 'greater than' block time rule
	bool AuthDescriptorRuleTestRun7();

	// should succeed if operation is executed after timestamp defined by 'greater than' block time rule
	bool AuthDescriptorRuleTestRun8();

	// should fail if operation is executed before block defined by 'greater than' block height rule
	bool AuthDescriptorRuleTestRun9();

	// should succeed if operation is executed after block defined by 'greater than' block height rule
	bool AuthDescriptorRuleTestRun10();

};

//#endif // FT3_TESTS
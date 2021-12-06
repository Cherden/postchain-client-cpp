#pragma once 

//#ifdef FT3_TESTS

#include <functional>
#include <string>
#include "../../src/PostchainClient/common.h"
#include "../../src/FT3/forward_declarations.h"
#include "../../src/FT3/User/AuthDescriptor/auth_descriptor_rule.h"
#include "../../src/FT3/User/asset.h"
#include "../../src/FT3/Core/Blockchain/blockchain_session.h"
#include "../../src/PostchainClient/postchain_util.h"
#include "../TestUtil/blockchain_util.h"
#include "../TestUtil/test_user.h"
#include "../TestUtil/test_util.h"
#include "../TestUtil/account_builder.h"

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

	// should be able to create complex rules
	bool AuthDescriptorRuleTestRun11();

	// should fail if block heights defined by 'greater than' and 'less than' block height rules are less than current block height
	bool AuthDescriptorRuleTestRun12();

	// should fail if block times defined by 'greater than' and 'less than' block time rules are in the past
	bool AuthDescriptorRuleTestRun13();

	// should succeed if current time is within period defined by 'greater than' and 'less than' block time rules
	bool AuthDescriptorRuleTestRun14();

	// should succeed if current time is within period defined by 'greater than' and 'less than' block time rules
	bool AuthDescriptorRuleTestRun15();

	// shouldn't delete non-expired auth descriptor
	bool AuthDescriptorRuleTestRun16();

	// should delete only expired auth descriptor if multiple expiring descriptors exist
	bool AuthDescriptorRuleTestRun17();

	// should add auth descriptors
	bool AuthDescriptorRuleTestRun18();

	// should delete auth descriptors
	bool AuthDescriptorRuleTestRun19();

	// should fail when deleting an auth descriptor which is not owned by the account
	bool AuthDescriptorRuleTestRun20();

	// should delete auth descriptor
	bool AuthDescriptorRuleTestRun21();

	// Should be able to create same rules with different value
	bool AuthDescriptorRuleTestRun22();

	// shouldn't be able to create too many rules
	bool AuthDescriptorRuleTestRun23();
};

//#endif // FT3_TESTS
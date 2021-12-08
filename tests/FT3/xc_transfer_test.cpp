#include "xc_transfer_test.h"
#include "../../src/FT3/User/AuthDescriptor/auth_descriptor_rule.h"
#include "../../src/FT3/User/AuthDescriptor/multi_signature_auth_descriptor.h"
#include "../../src/FT3/User/asset.h"
#include "../../src/FT3/User/asset_balance.h"
#include "../../src/FT3/User/user.h"
#include "../../src/FT3/User/account.h"
#include "../../src/FT3/User/account_operations.h"
#include "../../src/FT3/User/account_dev_operations.h"
#include "../../src/FT3/Core/transaction_builder.h"
#include "../../src/FT3/Core/transaction.h"
#include "../../src/FT3/Core/Blockchain/blockchain_session.h"
#include "../../src/FT3/Core/Blockchain/blockchain_info.h"
#include "../../src/FT3/Core/Blockchain/rate_limit_info.h"
#include "../../src/FT3/User/rate_limit.h"
#include "../../src/FT3/Core/key_pair.h"
#include "../../src/PostchainClient/postchain_util.h"
#include "../TestUtil/blockchain_util.h"
#include "../TestUtil/test_user.h"
#include "../TestUtil/test_util.h"
#include "../TestUtil/account_builder.h"

void XCTransferTest::DefaultErrorHandler(std::string error)
{
	chromia::postchain::PostchainUtil::Log(error);
};


void XCTransferTest::SetupBlockchain()
{
	std::function<void(std::shared_ptr<Blockchain>)> on_success = [&](std::shared_ptr<Blockchain> blockchain) {
		this->blockchain_ = blockchain;
	};
	BlockchainUtil::GetDefaultBlockchain(on_success, this->DefaultErrorHandler);
	return;
}


bool XCTransferTest::XCTransferTestRun1()
{
	SetupBlockchain();
	if (blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	Asset::Register(
		TestUtil::GenerateAssetName(),
		TestUtil::GenerateId(),
		blockchain_,
		[&asset](std::shared_ptr<Asset> _asset) {asset = _asset; },
		DefaultErrorHandler
	);
	if (asset == nullptr) return false;

	std::string destination_chain_id = TestUtil::GenerateId();
	std::string destination_account_id = TestUtil::GenerateId();
	std::shared_ptr<User> user = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	account_builder->WithParticipants({ user->key_pair_ });
	account_builder->WithBalance(asset, 100);
	account_builder->WithPoints(1);

	std::shared_ptr<Account> account = nullptr;
	account_builder->Build([&account](std::shared_ptr<Account> _account) {account = _account; });
	if (account == nullptr) return false;
	
	account->XcTransfer(destination_chain_id, destination_account_id, asset->id_, 10, EmptyCallback, DefaultErrorHandler);
	
	std::shared_ptr<AssetBalance> account_balance = nullptr;
	AssetBalance::GetByAccountAndAssetId(
		account->id_,
		asset->id_,
		blockchain_,
		[&account_balance](std::shared_ptr<AssetBalance> balance) {account_balance = balance; },
		DefaultErrorHandler
	);

	std::shared_ptr<AssetBalance> chain_balance = nullptr;
	AssetBalance::GetByAccountAndAssetId(
		TestUtil::BlockchainAccountId(destination_chain_id),
		asset->id_,
		blockchain_,
		[&chain_balance](std::shared_ptr<AssetBalance> balance) {chain_balance = balance; },
		DefaultErrorHandler
	);
	
	if (account_balance->amount_ != 90) return false;
	if (chain_balance->amount_ != 10) return false;

	return true;
}

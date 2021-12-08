#include "transfer_test.h"
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

void TransferTest::DefaultErrorHandler(std::string error)
{
	chromia::postchain::PostchainUtil::Log(error);
};


void TransferTest::SetupBlockchain()
{
	std::function<void(std::shared_ptr<Blockchain>)> on_success = [&](std::shared_ptr<Blockchain> blockchain) {
		this->blockchain_ = blockchain;
	};
	BlockchainUtil::GetDefaultBlockchain(on_success, this->DefaultErrorHandler);
	return;
}


bool TransferTest::TransferTestRun1()
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

	std::shared_ptr<User> user = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	account_builder->WithParticipants({user->key_pair_});
	account_builder->WithBalance(asset, 200);
	account_builder->WithPoints(1);

	std::shared_ptr<Account> account1 = nullptr;
	account_builder->Build([&account1](std::shared_ptr<Account> _account) {account1 = _account; });
	if (account1 == nullptr) return false;

	std::shared_ptr<AccountBuilder> account_builder_2 = AccountBuilder::CreateAccountBuilder(blockchain_);
	std::shared_ptr<Account> account2 = nullptr;
	account_builder_2->Build([&account2](std::shared_ptr<Account> _account) {account2 = _account; });
	if (account2 == nullptr) return false;

	account1->Transfer(account2->id_, asset->id_, 10, EmptyCallback, DefaultErrorHandler);
	
	std::shared_ptr<AssetBalance> asset_balance_1 = nullptr;
	AssetBalance::GetByAccountAndAssetId(
		account1->id_,
		asset->id_,
		blockchain_,
		[&asset_balance_1](std::shared_ptr<AssetBalance> _balance) { asset_balance_1 = _balance; },
		DefaultErrorHandler
	);

	std::shared_ptr<AssetBalance> asset_balance_2 = nullptr;
	AssetBalance::GetByAccountAndAssetId(
		account2->id_,
		asset->id_,
		blockchain_,
		[&asset_balance_2](std::shared_ptr<AssetBalance> _balance) { asset_balance_2 = _balance; },
		DefaultErrorHandler
	);

	if (asset_balance_1->amount_ != 190) return false;
	if (asset_balance_2->amount_ != 10) return false;

	return true;
}


bool TransferTest::TransferTestRun2()
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

	std::shared_ptr<User> user = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	account_builder->WithParticipants({ user->key_pair_ });
	account_builder->WithBalance(asset, 5);

	std::shared_ptr<Account> account1 = nullptr;
	account_builder->Build([&account1](std::shared_ptr<Account> _account) {account1 = _account; });
	if (account1 == nullptr) return false;

	std::shared_ptr<AccountBuilder> account_builder_2 = AccountBuilder::CreateAccountBuilder(blockchain_);
	std::shared_ptr<Account> account2 = nullptr;
	account_builder_2->Build([&account2](std::shared_ptr<Account> _account) {account2 = _account; });
	if (account2 == nullptr) return false;

	bool successfully = false;
	account1->Transfer(account2->id_, asset->id_, 10, [&successfully]() { successfully = true; }, DefaultErrorHandler);

	if (successfully) return false;

	return true;
}


bool TransferTest::TransferTestRun3()
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

	std::shared_ptr<User> user = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	account_builder->WithAuthFlags({ FlagsType::eAccount });
	account_builder->WithParticipants({ user->key_pair_ });
	account_builder->WithBalance(asset, 200);
	account_builder->WithPoints(1);

	std::shared_ptr<Account> account1 = nullptr;
	account_builder->Build([&account1](std::shared_ptr<Account> _account) {account1 = _account; });
	if (account1 == nullptr) return false;

	std::shared_ptr<AccountBuilder> account_builder_2 = AccountBuilder::CreateAccountBuilder(blockchain_);
	std::shared_ptr<Account> account2 = nullptr;
	account_builder_2->Build([&account2](std::shared_ptr<Account> _account) {account2 = _account; });
	if (account2 == nullptr) return false;

	bool successfully = false;
	account1->Transfer(account2->id_, asset->id_, 10, [&successfully]() { successfully = true; }, DefaultErrorHandler);

	if (successfully) return false;

	return true;
}


bool TransferTest::TransferTestRun4()
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

	std::shared_ptr<User> user1 = TestUser::SingleSig();
	std::shared_ptr<User> user2 = TestUser::SingleSig();
	std::shared_ptr<User> user3 = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user1);
	account_builder->WithParticipants({ user1->key_pair_ });
	account_builder->WithBalance(asset, 200);
	account_builder->WithPoints(0);

	std::shared_ptr<Account> account1 = nullptr;
	account_builder->Build([&account1](std::shared_ptr<Account> _account) {account1 = _account; });
	if (account1 == nullptr) return false;

	std::shared_ptr<AuthDescriptor> multisig = std::make_shared<MultiSignatureAuthDescriptor>(
		std::vector<std::vector<byte>> {user2->key_pair_->pub_key_, user3->key_pair_->pub_key_},
		2,
		std::vector<FlagsType> { FlagsType::eAccount, FlagsType::eTransfer }
	);

	auto tx_builder = blockchain_->NewTransactionBuilder();
	tx_builder->Add(AccountDevOperations::Register(multisig));
	auto tx = tx_builder->Build(multisig->Signers(), DefaultErrorHandler);
	tx->Sign(user2->key_pair_);
	tx->Sign(user3->key_pair_);
	tx->PostAndWait([] (std::string content) {});

	account1->Transfer(multisig->ID(), asset->id_, 10, EmptyCallback, DefaultErrorHandler);

	std::shared_ptr<AssetBalance> asset_balance_1 = nullptr;
	AssetBalance::GetByAccountAndAssetId(
		account1->id_,
		asset->id_,
		blockchain_,
		[&asset_balance_1](std::shared_ptr<AssetBalance> _balance) { asset_balance_1 = _balance; },
		DefaultErrorHandler
	);

	std::shared_ptr<AssetBalance> asset_balance_2 = nullptr;
	AssetBalance::GetByAccountAndAssetId(
		multisig->ID(),
		asset->id_,
		blockchain_,
		[&asset_balance_2](std::shared_ptr<AssetBalance> _balance) { asset_balance_2 = _balance; },
		DefaultErrorHandler
	);

	if (asset_balance_1->amount_ != 190) return false;
	if (asset_balance_2->amount_ != 10) return false;

	return true;
}


bool TransferTest::TransferTestRun5()
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

	std::shared_ptr<User> user = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	account_builder->WithParticipants({ user->key_pair_ });
	account_builder->WithBalance(asset, 200);
	account_builder->WithPoints(1);

	std::shared_ptr<Account> account = nullptr;
	account_builder->Build([&account](std::shared_ptr<Account> _account) {account = _account; });
	if (account == nullptr) return false;

	account->BurnTokens(asset->id_, 10, EmptyCallback, DefaultErrorHandler);
	std::shared_ptr<AssetBalance> asset_balance = account->GetAssetById(asset->id_);

	if (asset_balance->amount_ != 190) return false;

	return true;
}
#include "asset_balance_test.h"
#include "../TestUtil/test_util.h"
#include "../../src/FT3/User/asset.h"
#include "../../src/FT3/User/account.h"
#include "../../src/FT3/User/asset_balance.h"


void AssetBalanceTest::DefaultErrorHandler(std::string error)
{
	chromia::postchain::PostchainUtil::Log(error);
};


void AssetBalanceTest::SetupBlockchain()
{
	std::function<void(std::shared_ptr<Blockchain>)> on_success = [&](std::shared_ptr<Blockchain> blockchain) {
		this->blockchain_ = blockchain;
	};
	BlockchainUtil::GetDefaultBlockchain(on_success, this->DefaultErrorHandler);
	return;
}

bool AssetBalanceTest::AssetBalanceTestRun()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset1 = nullptr;
	Asset::Register(
		TestUtil::GenerateAssetName(), 
		TestUtil::GenerateId(), blockchain_, 
		[&asset1](std::shared_ptr<Asset> _asset) { asset1 = _asset; }, 
		DefaultErrorHandler);

	std::shared_ptr<Asset> asset2 = nullptr;
	Asset::Register(
		TestUtil::GenerateAssetName(),
		TestUtil::GenerateId(), blockchain_,
		[&asset2](std::shared_ptr<Asset> _asset) { asset2 = _asset; },
		DefaultErrorHandler);

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_);
	std::shared_ptr<Account> account = nullptr;
	account_builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; }, this->DefaultErrorHandler);

	AssetBalance::GiveBalance(account->id_, asset1->id_, 10, blockchain_, EmptyCallback, DefaultErrorHandler);
	AssetBalance::GiveBalance(account->id_, asset2->id_, 20, blockchain_, EmptyCallback, DefaultErrorHandler);

	std::vector<std::shared_ptr<AssetBalance>> balances;
	AssetBalance::GetByAccountId(
		account->id_, 
		blockchain_, 
		[&balances](std::vector<std::shared_ptr<AssetBalance>> _balances) { balances = _balances; },
		DefaultErrorHandler);

	return 2 == balances.size();
}
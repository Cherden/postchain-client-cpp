#include "asset_test.h"
#include "../TestUtil/test_util.h"
#include "../../src/FT3/User/asset.h"

void AssetTest::DefaultErrorHandler(std::string error)
{
	chromia::postchain::PostchainUtil::Log(error);
};


void AssetTest::SetupBlockchain()
{
	std::function<void(std::shared_ptr<Blockchain>)> on_success = [&](std::shared_ptr<Blockchain> blockchain) {
		this->blockchain_ = blockchain;
	};
	BlockchainUtil::GetDefaultBlockchain(on_success, this->DefaultErrorHandler);
	return;
}

bool AssetTest::AssetTestRun1()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	Asset::Register(
		TestUtil::GenerateAssetName(),
		TestUtil::GenerateId(),
		blockchain_,
		[&asset](std::shared_ptr<Asset> _asset) { asset = _asset; },
		DefaultErrorHandler
	);

	return asset != nullptr;

	return true;
}

bool AssetTest::AssetTestRun2()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::string asset_name = TestUtil::GenerateAssetName();

	std::shared_ptr<Asset> asset = nullptr;
	Asset::Register(
		asset_name,
		TestUtil::GenerateId(),
		blockchain_,
		[&asset](std::shared_ptr<Asset> _asset) { asset = _asset; },
		DefaultErrorHandler
	);

	vector<std::shared_ptr<Asset>> assets;
	Asset::GetByName(
		asset_name, 
		blockchain_, 
		[&assets](vector<std::shared_ptr<Asset>> _assets) { assets = _assets; }, 
		DefaultErrorHandler);

	if (1 != assets.size()) return false;
	if (asset->name_.compare(assets[0]->name_) != 0) return false;

	return true;
}

bool AssetTest::AssetTestRun3()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::string asset_name = TestUtil::GenerateAssetName();
	std::string test_chain_id = TestUtil::GenerateId();

	std::shared_ptr<Asset> asset = nullptr;
	Asset::Register(
		asset_name,
		test_chain_id,
		blockchain_,
		[&asset](std::shared_ptr<Asset> _asset) { asset = _asset; },
		DefaultErrorHandler
	);

	std::shared_ptr<Asset> expected_asset = nullptr;
	Asset::GetById(asset->id_, blockchain_, [&expected_asset](std::shared_ptr<Asset> _asset) { expected_asset = _asset; }, DefaultErrorHandler);

	if (asset_name.compare(expected_asset->name_) != 0) return false;
	if (PostchainUtil::ToUpper(asset->id_).compare(PostchainUtil::ToUpper(expected_asset->id_)) != 0) return false;
	if (PostchainUtil::ToUpper(test_chain_id).compare(PostchainUtil::ToUpper(expected_asset->issuing_chain_rid_)) != 0) return false;

	return true;
}

bool AssetTest::AssetTestRun4()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset1 = nullptr;
	Asset::Register(TestUtil::GenerateAssetName(), TestUtil::GenerateId(), blockchain_, [&asset1](std::shared_ptr<Asset> _asset) { asset1 = _asset; }, DefaultErrorHandler);
	std::shared_ptr<Asset> asset2 = nullptr;
	Asset::Register(TestUtil::GenerateAssetName(), TestUtil::GenerateId(), blockchain_, [&asset2](std::shared_ptr<Asset> _asset) { asset2 = _asset; }, DefaultErrorHandler);
	std::shared_ptr<Asset> asset3 = nullptr;
	Asset::Register(TestUtil::GenerateAssetName(), TestUtil::GenerateId(), blockchain_, [&asset3](std::shared_ptr<Asset> _asset) { asset3 = _asset; }, DefaultErrorHandler);
	
	std::vector<std::shared_ptr<Asset>> expected_assets;
	Asset::GetAssets(blockchain_, [&expected_assets](std::vector<std::shared_ptr<Asset>> _assets) { expected_assets = _assets; }, DefaultErrorHandler);
	std::vector<std::string> asset_names;
	for (auto &asset : expected_assets)
	{
		asset_names.push_back(asset->name_);
	}
	if (std::find(asset_names.begin(), asset_names.end(), asset1->name_) == asset_names.end()) return false;
	if (std::find(asset_names.begin(), asset_names.end(), asset2->name_) == asset_names.end()) return false;
	if (std::find(asset_names.begin(), asset_names.end(), asset3->name_) == asset_names.end()) return false;

	return true;
}
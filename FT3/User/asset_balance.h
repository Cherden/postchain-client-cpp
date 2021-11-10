#pragma once

#include <memory>
#include "../forward_declarations.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class AssetBalance
{
public:
	long amount_;
    std::shared_ptr<Asset> asset_;

	AssetBalance(long amount, std::shared_ptr<Asset> asset);

	AssetBalance(std::string id, std::string name, long amount, std::string chain_id);

	static void GetByAccountId(std::string id, std::shared_ptr<Blockchain> blockchain,
		std::function<void(std::vector<std::shared_ptr<AssetBalance>>)> on_success, std::function<void(std::string)> on_error);

	static void GetByAccountAndAssetId(std::string account_id, std::string asset_id, std::shared_ptr<Blockchain> blockchain,
		std::function<void(std::shared_ptr<AssetBalance>)> on_success, std::function<void(string)> on_error);

	static void GiveBalance(std::string account_id, std::string asset_id, int amount, std::shared_ptr<Blockchain> blockchain,
		std::function<void()> on_success, std::function<void(string)> on_error);
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
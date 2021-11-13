#include "asset_balance.h"
#include "asset.h"
#include "account_operations.h"
#include "../Core/transaction_builder.h"
#include "../Core/Blockchain/blockchain.h"
#include "../Core/transaction.h"
#include "../Core/operation.h"

namespace chromia {
namespace postchain {
namespace ft3 {

AssetBalance::AssetBalance(long amount, std::shared_ptr<Asset> asset)
{
	this->amount_ = amount;
	this->asset_ = asset;
}

AssetBalance::AssetBalance(std::string id, std::string name, long amount, std::string chain_id)
{
	this->amount_ = amount;
	this->asset_ = std::make_shared<Asset>(name, chain_id);
}

void AssetBalance::GetByAccountId(std::string id, std::shared_ptr<Blockchain> blockchain,
	std::function<void(std::vector<std::shared_ptr<AssetBalance>>)> on_success, std::function<void(std::string)> on_error)
{
	std::vector<QueryObject> query_objects;
	query_objects.push_back(QueryObject("account_id", AbstractValueFactory::Build(id)));

	std::function<void(std::string)> on_success_wrapper = [on_success, on_error](std::string content) {
		// TO-DO check parsing
		nlohmann::json json_content = nlohmann::json::parse(content);
		if (json_content.contains("id") && json_content.contains("name") && json_content.contains("chain_id") && json_content.contains("amount"))
		{
			std::string amount_str = json_content["amount"];
			long amount = atoi(amount_str.c_str());

			std::shared_ptr<AssetBalance> asset = std::make_shared<AssetBalance>(json_content["id"], json_content["name"], amount, json_content["chain_id"]);
			on_success({ asset });
		}
		else
		{
			on_error("Assrt::GetById failed, corrupted resposne");
		}
	};

	blockchain->Query("ft3.get_asset_balances", query_objects, on_success_wrapper, on_error);
}

void AssetBalance::GetByAccountAndAssetId(std::string account_id, std::string asset_id, std::shared_ptr<Blockchain> blockchain,
	std::function<void(std::shared_ptr<AssetBalance>)> on_success, std::function<void(string)> on_error)
{
	std::vector<QueryObject> query_objects;
	query_objects.push_back(QueryObject("account_id", AbstractValueFactory::Build(account_id)));
	query_objects.push_back(QueryObject("asset_id", AbstractValueFactory::Build(asset_id)));

	std::function<void(std::string)> on_success_wrapper = [on_success, on_error](std::string content) {
		// TO-DO check parsing
		nlohmann::json json_content = nlohmann::json::parse(content);
		if (json_content.contains("id") && json_content.contains("name") && json_content.contains("chain_id") && json_content.contains("amount"))
		{
			std::string amount_str = json_content["amount"];
			long amount = atoi(amount_str.c_str());

			std::shared_ptr<AssetBalance> asset = std::make_shared<AssetBalance>(json_content["id"], json_content["name"], amount, json_content["chain_id"]);
			on_success({ asset });
		}
		else
		{
			on_error("Assrt::GetById failed, corrupted resposne");
		}
	};

	blockchain->Query("ft3.get_asset_balances", query_objects, on_success_wrapper, on_error);
}

void AssetBalance::GiveBalance(std::string account_id, std::string asset_id, int amount, std::shared_ptr<Blockchain> blockchain,
	std::function<void()> on_success, std::function<void(string)> on_error)
{
	std::shared_ptr<TransactionBuilder> tx_builder = blockchain->NewTransactionBuilder();

	std::shared_ptr<ArrayValue> op_args;
	op_args->Add(AbstractValueFactory::Build(asset_id));
	op_args->Add(AbstractValueFactory::Build(account_id));
	op_args->Add(AbstractValueFactory::Build(amount));
	tx_builder->Add(ft3::Operation::Op("ft3.dev_give_balance", op_args));
	tx_builder->Add(AccountOperations::Nop());

	std::shared_ptr<ft3::Transaction> tx = tx_builder->Build(std::vector<std::vector<byte>>(), on_error);

	std::function<void()> on_success_wrapper = [on_success]() {
		on_success();
	};

	tx->PostAndWait(on_success_wrapper);
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
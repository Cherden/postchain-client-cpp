#include "asset.h"
#include "../Core/transaction_builder.h"
#include "../Core/Blockchain/blockchain.h"
#include "../Core/transaction.h"
#include "../Core/operation.h"
#include "../../PostchainClient/GTX/gtx_value.h"

namespace chromia {
namespace postchain {
namespace ft3 {

Asset::Asset(std::string name, std::string chain_id)
{
	this->name_ = name;
	this->issuing_chain_rid_ = chain_id;
	this->id_ = HashId();
}

Asset::Asset(std::string id, std::string name, std::string issuing_chain_rid)
{
	this->id_ = id;
	this->name_ = name;
	this->issuing_chain_rid_ = issuing_chain_rid;
}

void Asset::Register(std::string name, std::string chain_id, std::shared_ptr<Blockchain> blockchain,
	std::function<void(std::shared_ptr<Asset>)> on_success,
	std::function<void(string)> on_error)
{
	std::shared_ptr<TransactionBuilder> tx_builder = blockchain->NewTransactionBuilder();

	std::shared_ptr<ArrayValue> op_args = AbstractValueFactory::EmptyArray();
	op_args->Add(AbstractValueFactory::Build(name));
	op_args->Add(AbstractValueFactory::Build(chain_id));
	tx_builder->Add(ft3::Operation::Op("ft3.dev_register_asset", op_args));

	std::shared_ptr<ft3::Transaction> tx = tx_builder->Build(std::vector<std::vector<BYTE>>(), on_error);

	std::function<void(std::string)> on_success_wrapper = [on_success, name, chain_id](std::string content) {
		on_success(std::make_shared<Asset>(name, chain_id));
	};

	tx->PostAndWait(on_success_wrapper);
}

void Asset::GetByName(std::string name, std::shared_ptr<Blockchain> blockchain,
	std::function<void(std::vector<std::shared_ptr<Asset>>)> on_success, std::function<void(string)> on_error)
{
	std::vector<QueryObject> query_objects;
	query_objects.push_back(QueryObject("name", AbstractValueFactory::Build(name)));

	std::function<void(std::string)> on_success_wrapper = [on_success, on_error](std::string content) {
		nlohmann::json json_content = nlohmann::json::parse(content);
		std::vector<std::shared_ptr<Asset>> assets;
		for (auto &item : json_content)
		{
			std::shared_ptr<Asset> asset = std::make_shared<Asset>(item["id"], item["name"], item["issuing_chain_rid"]);
			assets.push_back(asset);
		}
		on_success(assets);
	};

	blockchain->Query("ft3.get_asset_by_name", query_objects, on_success_wrapper, on_error);
}

void Asset::GetById(string id, std::shared_ptr<Blockchain> blockchain,
	std::function<void(std::shared_ptr<Asset>)> on_success, std::function<void(string)> on_error)
{
	std::vector<QueryObject> query_objects;
	query_objects.push_back(QueryObject("asset_id", AbstractValueFactory::Build(id)));

	std::function<void(std::string)> on_success_wrapper = [on_success, on_error](std::string content) {
		nlohmann::json json_content = nlohmann::json::parse(content);
		if (json_content.contains("id") && json_content.contains("name") && json_content.contains("issuing_chain_rid"))
		{
			std::shared_ptr<Asset> asset = std::make_shared<Asset>(json_content["id"], json_content["name"], json_content["issuing_chain_rid"]);
			on_success(asset);
		}
		else
		{
			on_error("Asset::GetById failed, corrupted resposne");
		}
	};

	blockchain->Query("ft3.get_asset_by_id", query_objects, on_success_wrapper, on_error);
}

void Asset::GetAssets(std::shared_ptr<Blockchain> blockchain,
	std::function<void(std::vector<std::shared_ptr<Asset>>)> on_success, std::function<void(string)> on_error)
{
	std::function<void(std::string)> on_success_wrapper = [on_success, on_error](std::string content) {
		nlohmann::json json_content = nlohmann::json::parse(content);
		std::vector<std::shared_ptr<Asset>> assets;
		for (auto &item : json_content)
		{
			std::shared_ptr<Asset> asset = std::make_shared<Asset>(item["id"], item["name"], item["issuing_chain_rid"]);
			assets.push_back(asset);
		}
		on_success(assets);
	};

	blockchain->Query("ft3.get_all_assets", {}, on_success_wrapper, on_error);
}

std::string Asset::HashId()
{
	std::shared_ptr<ArrayValue> body = AbstractValueFactory::EmptyArray();
	body->Add(AbstractValueFactory::Build(this->name_));
	body->Add(AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector(this->issuing_chain_rid_)));

	std::vector<BYTE> hash = AbstractValue::Hash(body);
	std::string hash_hex = PostchainUtil::ByteVectorToHexString(hash);
	return hash_hex;
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
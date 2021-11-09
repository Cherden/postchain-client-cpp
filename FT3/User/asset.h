#pragma once

#include <string>
#include "../../src/GTV/abstract_value_factory.h"
#include "../../src/postchain_util.h"
#include "../../src/common.h"
#include "../Core/Blockchain/blockchain.h"
#include "../Core/transaction.h"
#include "../Core/operation.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {
class Asset
{
public:
	std::string id_;
    std::string name_;

    //[JsonProperty(PropertyName = "issuing_chain_rid")]
    std::string issuing_chain_rid_;

    Asset(std::string name, std::string chain_id)
    {
        this->name_ = name;
        this->issuing_chain_rid_ = chain_id;
        this->id_ = HashId();
    }

    //[JsonConstructor]
    Asset(std::string id, std::string name, std::string issuing_chain_rid)
    {
        this->id_ = id;
        this->name_ = name;
        this->issuing_chain_rid_ = issuing_chain_rid;
    }

    static void Register(std::string name, std::string chain_id, std::shared_ptr<Blockchain> blockchain, 
		std::function<void(std::shared_ptr<Asset>)> on_success, 
		std::function<void(string)> on_error)
    {
		std::shared_ptr<TransactionBuilder> tx_builder = blockchain->TransactionBuilder();

		std::shared_ptr<ArrayValue> op_args;
		op_args->Add(AbstractValueFactory::Build(name));
		op_args->Add(AbstractValueFactory::Build(chain_id));
		tx_builder->Add(ft3::Operation::Op("ft3.dev_register_asset", op_args));
		
		std::shared_ptr<ft3::Transaction> tx = tx_builder->Build(std::vector<std::vector<byte>>(), on_error);

		std::function<void()> on_success_wrapper = [on_success, name, chain_id](){
			on_success(std::make_shared<Asset>(name, chain_id));
		};

		tx->PostAndWait(on_success_wrapper);
    }

    static void GetByName(std::string name, std::shared_ptr<Blockchain> blockchain, 
		std::function<void(std::vector<std::shared_ptr<Asset>>)> on_success, std::function<void(string)> on_error)
    {
		std::vector<QueryObject> query_objects;
		query_objects.push_back(QueryObject("name", AbstractValueFactory::Build(name)));

		std::function<void(std::string)> on_success_wrapper = [on_success, on_error](std::string content) {
			nlohmann::json json_content = nlohmann::json::parse(content);
			if (json_content.contains("id") && json_content.contains("name") && json_content.contains("issuing_chain_rid")) 
			{
				std::shared_ptr<Asset> asset = std::make_shared<Asset>(json_content["id"], json_content["name"], json_content["issuing_chain_rid"]);
				std::vector<std::shared_ptr<Asset>> assets;
				assets.push_back(asset);
				on_success(assets);
			}
			else 
			{
				on_error("Asset::GetByName failed, corrupted resposne");
			}
		};

		blockchain->Query("ft3.get_asset_by_name", query_objects, on_success_wrapper, on_error);
    }

    static void GetById(string id, std::shared_ptr<Blockchain> blockchain,
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
				on_error("Assrt::GetById failed, corrupted resposne");
			}
		};

		blockchain->Query("ft3.get_asset_by_id", query_objects, on_success_wrapper, on_error);
    }

	static void GetAssets(string id, std::shared_ptr<Blockchain> blockchain,
		std::function<void(std::vector<std::shared_ptr<Asset>>)> on_success, std::function<void(string)> on_error)
	{
		std::function<void(std::string)> on_success_wrapper = [on_success, on_error](std::string content) {
			nlohmann::json json_content = nlohmann::json::parse(content);
			if (json_content.contains("id") && json_content.contains("name") && json_content.contains("issuing_chain_rid"))
			{
				std::shared_ptr<Asset> asset = std::make_shared<Asset>(json_content["id"], json_content["name"], json_content["issuing_chain_rid"]);
				std::vector<std::shared_ptr<Asset>> assets;
				assets.push_back(asset);
				on_success(assets);
			}
			else
			{
				on_error("Assrt::GetAssets failed, corrupted resposne");
			}
		};

		blockchain->Query("ft3.get_all_assets", {}, on_success_wrapper, on_error);
	}

private:
	std::string HashId()
	{
		std::shared_ptr<ArrayValue> body = AbstractValueFactory::EmptyArray();
		body->Add(AbstractValueFactory::Build(this->name_));
		body->Add(AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector(this->issuing_chain_rid_)));

		std::vector<byte> hash = AbstractValue::Hash(body);
		std::string hash_hex = PostchainUtil::ByteVectorToHexString(hash);
		return hash_hex;
	}
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
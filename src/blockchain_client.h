#ifndef POSTCHAIN_CLIENT_BLOCKCHAINCLIENT_H_
#define POSTCHAIN_CLIENT_BLOCKCHAINCLIENT_H_

#include <string>
#include <vector>
#include "SECP256K/include/secp256k1.h"
#include "query.h"
#include "../../nlohmann/json.hpp"
#include "./transaction.h"
#include "./postchain_util.h"
#include "../HTTP/http-request.h"

namespace chromia {
namespace postchain {
namespace client {
class BlockchainClient
{
public:
	std::string blockchain_rid_;
	std::string base_url_;

	void Setup(std::string blockchain_rid, std::string base_url)
	{
		this->blockchain_rid_ = blockchain_rid;
		this->base_url_ = base_url;
	}

	std::shared_ptr<Transaction> NewTransaction(std::vector<std::vector<byte>> signers)
	{
		Gtx new_gtx(this->blockchain_rid_);

		for(std::vector<byte> signer : signers)
		{
			new_gtx.AddSignerToGtx(signer);
		}

		return std::make_shared<Transaction>(new_gtx, this->base_url_, this->blockchain_rid_);
	}

	/*std::vector<QueryObject> queryObjects;
	queryObjects.push_back(QueryObject("name", AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username))));*/
	void Query(std::string query_name, std::vector<QueryObject> query_objects, std::function<void(int, std::string)> on_response)
	{
		std::string payload = PostchainUtil::QueryToJSONString("check_user", query_objects);
		std::string url = this->base_url_ + "/query" + this->blockchain_rid_;
		http::HttpRequest http_request(url, on_response);
	}

public:
	std::string blockchain_rid_;
	int chain_id_;
	std::string base_url_;

};
} // namespace client
} // namespace postchain
} // namespace chromia

#endif //POSTCHAIN_CLIENT_BLOCKCHAINCLIENT_H_
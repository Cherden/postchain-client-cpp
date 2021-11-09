#ifndef POSTCHAIN_CLIENT_BLOCKCHAINCLIENT_H_
#define POSTCHAIN_CLIENT_BLOCKCHAINCLIENT_H_

#include <string>
#include <vector>
#include "SECP256K/include/secp256k1.h"
#include "query.h"
#include "../../nlohmann/json.hpp"
#include "./postchain_transaction.h"
#include "./postchain_util.h"
#include "../HTTP/httprequest.h"

namespace chromia {
namespace postchain {
namespace client {
class BlockchainClient
{
public:
	std::string blockchain_rid_;
	int chain_id_;
	std::string base_url_;

	BlockchainClient() {};

	void Setup(std::string blockchain_rid, std::string base_url)
	{
		this->blockchain_rid_ = blockchain_rid;
		this->base_url_ = base_url;
	}

	std::shared_ptr<PostchainTransaction> NewTransaction(std::vector<std::vector<byte>> signers) // TO-DO add on-error callback
	{
		std::shared_ptr<Gtx> new_gtx = std::make_shared<Gtx>(this->blockchain_rid_);

		for(std::vector<byte> signer : signers)
		{
			new_gtx->AddSignerToGtx(signer);
		}

		return std::make_shared<PostchainTransaction>(new_gtx, this->base_url_, this->blockchain_rid_);
	}

	/*std::vector<QueryObject> queryObjects;
	queryObjects.push_back(QueryObject("name", AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username))));*/
	void Query(std::string query_name, std::vector<QueryObject> query_objects, 
		std::function<void(std::string)> on_success, std::function<void(std::string)> on_error)
	{
		std::string payload = PostchainUtil::QueryToJSONString(query_name, query_objects);
		std::string url = this->base_url_ + "/query" + this->blockchain_rid_;
		
		std::shared_ptr<UHttpRequest> http_request = UHttpRequest::BuildHttpRequest(url, on_success, on_error);
		http_request->SetContent(payload);
		http_request->Post();
	}

};
} // namespace client
} // namespace postchain
} // namespace chromia

#endif //POSTCHAIN_CLIENT_BLOCKCHAINCLIENT_H_
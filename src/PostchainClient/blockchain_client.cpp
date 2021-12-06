#include "blockchain_client.h"
#include "../nlohmann/json.hpp"

namespace chromia {
namespace postchain {
namespace client {

BlockchainClient::BlockchainClient() {};

BlockchainClient::~BlockchainClient() {};

void BlockchainClient::Setup(std::string blockchain_rid, std::string base_url)
{
	this->blockchain_rid_ = blockchain_rid;
	this->base_url_ = base_url;
}

std::shared_ptr<PostchainTransaction> BlockchainClient::NewTransaction(std::vector<std::vector<byte>> signers, std::function<void(std::string)> on_error)
{
	std::shared_ptr<Gtx> new_gtx = std::make_shared<Gtx>(this->blockchain_rid_);

	for (std::vector<byte> signer : signers)
	{
		new_gtx->AddSignerToGtx(signer);
	}

	return std::make_shared<PostchainTransaction>(new_gtx, this->base_url_, this->blockchain_rid_, on_error);
}


void BlockchainClient::Query(std::string query_name, std::vector<QueryObject> query_objects,
	std::function<void(std::string)> on_success, std::function<void(std::string)> on_error)
{
	std::string payload = PostchainUtil::QueryToJSONString(query_name, query_objects);
	std::string url = this->base_url_ + "/query/" + this->blockchain_rid_;
	postchain::http::HttpRequest::SendPostRequest(url, payload, on_success, on_error);
}

} // namespace client
} // namespace postchain
} // namespace chromia

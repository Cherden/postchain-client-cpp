#ifndef POSTCHAIN_CLIENT_BLOCKCHAINCLIENT_H_
#define POSTCHAIN_CLIENT_BLOCKCHAINCLIENT_H_

#include <string>
#include <vector>
#include "SECP256K/include/secp256k1.h"
#include "query.h"
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

	BlockchainClient();

	~BlockchainClient();

	void Setup(std::string blockchain_rid, std::string base_url);

	std::shared_ptr<PostchainTransaction> NewTransaction(std::vector<std::vector<BYTE>> signers, std::function<void(std::string)> on_error);

	void Query(std::string query_name, std::vector<QueryObject> query_objects,
		std::function<void(std::string)> on_success, std::function<void(std::string)> on_error);

};
} // namespace client
} // namespace postchain
} // namespace chromia

#endif //POSTCHAIN_CLIENT_BLOCKCHAINCLIENT_H_
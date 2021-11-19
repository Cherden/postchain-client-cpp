#pragma once

#include <string>
#include <memory>
#include "../../../src/blockchain_client.h"
#include "../../forward_declarations.h"

using namespace chromia::postchain::client;

namespace chromia {
namespace postchain {
namespace ft3 {

/**
* Note, this class is inherited from enable_shared_from_this<...> in order to be able init other instances wich needs shared_ptr<...> reference to this class
*/
class Blockchain : public std::enable_shared_from_this<Blockchain>
{
public:
	std::string id_;
	std::shared_ptr<BlockchainInfo> info_;
	std::shared_ptr<BlockchainClient> connection_;
	
	Blockchain(std::string id, std::shared_ptr<BlockchainInfo> info, std::shared_ptr<BlockchainClient> connection,
		std::shared_ptr<DirectoryService> directory_service);

	static void Initialize(string blockchain_rid, std::shared_ptr<DirectoryService> directory_service,
		std::function<void(std::shared_ptr<Blockchain>)> on_success, std::function<void(std::string)> on_error);

	std::shared_ptr<TransactionBuilder> NewTransactionBuilder();

	std::shared_ptr<BlockchainSession> NewSession(std::shared_ptr<User> user);

	void GetAccountsByParticipantId(string id, std::shared_ptr<User> user,
		std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error);
	
	void GetAccountsByAuthDescriptorId(string id, std::shared_ptr<User> user,
		std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error);

	void RegisterAccount(std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<User> user,
		std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error);

	void GetAssetsByName(string name, std::function<void(std::vector<std::shared_ptr<Asset>>)> on_success, std::function<void(std::string)> on_error);

	void GetAssetById(string id, std::function<void(std::shared_ptr<Asset>)> on_success, std::function<void(std::string)> on_error);

	void GetAllAssets(std::function<void(std::vector<std::shared_ptr<Asset>>)> on_success, std::function<void(std::string)> on_error);

	void LinkChain(string chain_id, std::function<void(std::string)> on_success, std::function<void(std::string)> on_error);

	void IsLinkedWithChain(string chain_id, std::function<void(bool)> on_success, std::function<void(std::string)> on_error);

	void GetLinkedChainsIds(std::function<void(std::vector<std::string>)> on_success, std::function<void(std::string)> on_error);

	void GetLinkedChains(std::function<void(std::vector<std::shared_ptr<Blockchain>>)> on_success, std::function<void(std::string)> on_error);

	void Query(string query_name, std::vector<QueryObject> query_objects, std::function<void(std::string)> on_success, std::function<void(std::string)> on_error);

	void Call(std::shared_ptr<ft3::Operation> operation, std::shared_ptr<User> user,
		std::function<void(std::string)> on_success, std::function<void(std::string)> on_error);
	
private:
	std::shared_ptr<DirectoryService> directory_service_;
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
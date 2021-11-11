#pragma once

#include "../../forward_declarations.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class BlockchainSession
{
public:
	std::shared_ptr<User> user_;
    std::shared_ptr<Blockchain> blockchain_;

	BlockchainSession(std::shared_ptr<User> user, std::shared_ptr<Blockchain> blockchain);

	void GetAccountById(string id, std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error);

	void GetAccountsByParticipantId(std::string id, std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error);

	void GetAccountsByAuthDescriptorId(string id, std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error);

	void Query(string query_name, std::vector<QueryObject> query_objects,
		std::function<void(std::string)> on_success, std::function<void(string)> on_error);
	
	void Call(std::shared_ptr<Operation> operation,
		std::function<void()> on_success, std::function<void(string)> on_error);
};

} // namespace ft3
} // namespace postchain
} // namespace chromia
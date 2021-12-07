#pragma once

#include "../../forward_declarations.h"
#include <functional>

using namespace chromia::postchain;

namespace chromia {
namespace postchain {
namespace ft3 {

/**
* Note, this class is inherited from enable_shared_from_this<...> in order to be able init other instances wich needs shared_ptr<...> reference to this class
*/
class BlockchainSession : public std::enable_shared_from_this<BlockchainSession>
{
public:
	std::shared_ptr<User> user_;
    std::shared_ptr<Blockchain> blockchain_;

	BlockchainSession(std::shared_ptr<User> user, std::shared_ptr<Blockchain> blockchain);

	void GetAccountById(std::string id, std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error);

	void GetAccountsByParticipantId(std::string id, std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error);

	void GetAccountsByAuthDescriptorId(std::string id, std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error);

	void Query(std::string query_name, std::vector<QueryObject> query_objects,
		std::function<void(std::string)> on_success, std::function<void(std::string)> on_error);
	
	void Call(std::shared_ptr<Operation> operation,
		std::function<void()> on_success, std::function<void(std::string)> on_error);
};

} // namespace ft3
} // namespace postchain
} // namespace chromia
#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "../../PostchainClient/common.h"
#include "../forward_declarations.h"

namespace chromia {
namespace postchain {
namespace ft3 {

/**
* Note, this class is inherited from enable_shared_from_this<...> in order to be able init other instances wich needs shared_ptr<...> reference to this class
*/
class TransactionBuilder : public std::enable_shared_from_this<TransactionBuilder>
{
public:
	std::shared_ptr<Blockchain> blockchain_;

	TransactionBuilder(std::shared_ptr<Blockchain> blockchain);

	std::shared_ptr<TransactionBuilder> Add(std::shared_ptr<ft3::Operation> operation);

	std::shared_ptr<Transaction> Build(std::vector<std::vector<BYTE>> signers, std::function<void(std::string)> on_error);

	std::shared_ptr<Transaction> BuildAndSign(std::shared_ptr<User> user, std::function<void(std::string)> on_error);

private:
	std::vector<std::shared_ptr<ft3::Operation>> operations_;
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
#pragma once

#include <memory>
#include <vector>
#include "../../src/common.h"
#include "../forward_declarations.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class TransactionBuilder
{
public:
	std::shared_ptr<Blockchain> blockchain_;

	TransactionBuilder(std::shared_ptr<Blockchain> blockchain);

	std::shared_ptr<TransactionBuilder> Add(std::shared_ptr<ft3::Operation> operation);

	std::shared_ptr<Transaction> Build(std::vector<std::vector<byte>> signers, std::function<void(std::string)> on_error);

	std::shared_ptr<Transaction> BuildAndSign(std::shared_ptr<User> user, std::function<void(std::string)> on_error);

private:
	std::vector<std::shared_ptr<ft3::Operation>> operations_;
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
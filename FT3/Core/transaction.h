#pragma once

#include <memory>
#include <vector>
#include "../../src/common.h"
#include "../../src/postchain_transaction.h"
#include "../Core/key_pair.h"

namespace chromia {
namespace postchain {
namespace ft3 {
class Transaction
{
private:
	std::shared_ptr<PostchainTransaction> _tx;

public:
	Transaction(std::shared_ptr<PostchainTransaction> tx);

	std::shared_ptr<Transaction> Sign(std::shared_ptr<KeyPair> keyPair);

	void Post();

	void PostAndWait(std::function<void()> on_success);

	std::vector<byte> Raw();
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
#pragma once

#include <memory>
#include <vector>
#include "../../src/common.h"
#include "../../src/postchain_transaction.h"
#include "../Core/key_pair.h"

namespace chromia {
namespace postchain {
namespace ft3 {

/**
* Note, this class is inherited from enable_shared_from_this<...> in order to be able init other instances wich needs shared_ptr<...> reference to this class
*/
class Transaction : public std::enable_shared_from_this<Transaction>
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
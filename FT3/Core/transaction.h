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
    Transaction(std::shared_ptr<PostchainTransaction> tx)
    {
        _tx = tx;
    }

    std::shared_ptr<Transaction> Sign(std::shared_ptr<KeyPair> keyPair)
    {
        this->_tx->Sign(keyPair->priv_key_, keyPair->pub_key_);
		return std::shared_ptr<Transaction>(this);
    }

    void Post()
    {
        return this->_tx->Post();
    }

    void PostAndWait(std::function<void()> on_success)
    {
		this->_tx->PostAndWait(on_success);
    }

	std::vector<byte> Raw()
    {
        return this->_tx->Encode();
    }
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
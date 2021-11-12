#pragma once

#include "transaction.h"

namespace chromia {
namespace postchain {
namespace ft3 {

Transaction::Transaction(std::shared_ptr<PostchainTransaction> tx)
{
	_tx = tx;
}

std::shared_ptr<Transaction> Transaction::Sign(std::shared_ptr<KeyPair> keyPair)
{
	this->_tx->Sign(keyPair->priv_key_, keyPair->pub_key_);
	return std::shared_ptr<Transaction>(this);
}

void Transaction::Post()
{
	return this->_tx->Post();
}

void Transaction::PostAndWait(std::function<void()> on_success)
{
	this->_tx->PostAndWait(on_success);
}

std::vector<byte> Transaction::Raw()
{
	return this->_tx->Encode();
}
} // namespace ft3
} // namespace postchain
} // namespace chromia
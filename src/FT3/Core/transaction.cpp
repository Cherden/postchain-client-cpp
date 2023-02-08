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
	return shared_from_this();
}

void Transaction::Post(std::function<void(std::string)> on_success)
{
	this->_tx->Post(on_success);
}

void Transaction::PostAndWait(std::function<void(std::string)> on_success)
{
	this->_tx->PostAndWait(on_success);
}

std::vector<BYTE> Transaction::Raw()
{
	return this->_tx->Encode();
}
} // namespace ft3
} // namespace postchain
} // namespace chromia
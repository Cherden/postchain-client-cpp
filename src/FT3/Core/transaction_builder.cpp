#include "transaction_builder.h"
#include "./Blockchain/blockchain.h"
#include "../User/user.h"
#include "../../PostchainClient/postchain_transaction.h"
#include "transaction.h"
#include "../Core/operation.h"

namespace chromia {
namespace postchain {
namespace ft3 {

TransactionBuilder::TransactionBuilder(std::shared_ptr<Blockchain> blockchain)
{
	this->blockchain_ = blockchain;
}

std::shared_ptr<TransactionBuilder> TransactionBuilder::Add(std::shared_ptr<ft3::Operation> operation)
{
	this->operations_.push_back(operation);
	return shared_from_this();
}

std::shared_ptr<Transaction> TransactionBuilder::Build(std::vector<std::vector<BYTE>> signers, std::function<void(std::string)> on_error)
{
	std::shared_ptr<PostchainTransaction> tx = this->blockchain_->connection_->NewTransaction(signers, on_error);
	for (auto &op : operations_)
	{
		tx->AddOperation(op->name_, op->args_);
	}
	return std::make_shared<Transaction>(tx);
}

std::shared_ptr<Transaction> TransactionBuilder::BuildAndSign(std::shared_ptr<User> user, std::function<void(std::string)> on_error)
{
	std::shared_ptr<Transaction> tx = this->Build(user->auth_descriptor_->Signers(), on_error);
	tx->Sign(user->key_pair_);
	return tx;
}


} // namespace ft3
} // namespace postchain
} // namespace chromia
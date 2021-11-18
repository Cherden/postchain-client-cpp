#pragma once

#include "blockchain_session.h"
#include "../../User/user.h"
#include "../../User/account.h"
#include "./blockchain.h"
#include "../operation.h"

namespace chromia {
namespace postchain {
namespace ft3 {

BlockchainSession::BlockchainSession(std::shared_ptr<User> user, std::shared_ptr<Blockchain> blockchain)
{
	this->user_ = user;
	this->blockchain_ = blockchain;
}

void BlockchainSession::GetAccountById(string id, std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{
	Account::GetById(id, shared_from_this(), on_success, on_error);
}

void BlockchainSession::GetAccountsByParticipantId(std::string id, std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error)
{
	Account::GetByParticipantId(id, shared_from_this(), on_success, on_error);
}

void BlockchainSession::GetAccountsByAuthDescriptorId(string id, std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error)
{
	Account::GetByAuthDescriptorId(id, shared_from_this(), on_success, on_error);
}

void BlockchainSession::Query(string query_name, std::vector<QueryObject> query_objects,
	std::function<void(std::string)> on_success, std::function<void(string)> on_error)
{
	this->blockchain_->Query(query_name, query_objects, on_success, on_error);
}

void BlockchainSession::Call(std::shared_ptr<Operation> operation,
	std::function<void()> on_success, std::function<void(string)> on_error)
{
	this->blockchain_->Call(operation, this->user_, on_success, on_error);
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
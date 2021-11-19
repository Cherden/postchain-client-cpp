#include "blockchain.h"
#include "blockchain_info.h"
#include "directory_service.h"
#include "../../Core/transaction_builder.h"
#include "../../Core/transaction.h"
#include "../../User/account.h"
#include "../../User/asset.h"
#include "../../User/user.h"
#include "../operation.h"
#include "../../../src/GTV/abstract_value_factory.h"
#include "../../User/account_operations.h"

namespace chromia {
namespace postchain {
namespace ft3 {

Blockchain::Blockchain(std::string id, std::shared_ptr<BlockchainInfo> info, std::shared_ptr<BlockchainClient> connection,
	std::shared_ptr<DirectoryService> directory_service)
{
	this->id_ = id;
	this->info_ = info;
	this->connection_ = connection;
	this->directory_service_ = directory_service;
}

void Blockchain::Initialize(string blockchain_rid, std::shared_ptr<DirectoryService> directory_service,
	std::function<void(std::shared_ptr<Blockchain>)> on_success, std::function<void(std::string)> on_error)
{
	std::shared_ptr<ChainConnectionInfo> chainConnectionInfo = directory_service->GetChainConnectionInfo(blockchain_rid);

	if (chainConnectionInfo == nullptr)
	{
		throw new std::exception("Cannot find details for chain"); // Add RID
	}

	std::shared_ptr<BlockchainClient> connection = std::make_shared<BlockchainClient>();
	connection->Setup(blockchain_rid, chainConnectionInfo->url_);

	BlockchainInfo::GetInfo(connection, [=](std::shared_ptr<BlockchainInfo> blockchain_info) {
		on_success(std::make_shared<Blockchain>(blockchain_rid, blockchain_info, connection, directory_service));
	}, on_error);

	return;
}

std::shared_ptr<TransactionBuilder> Blockchain::NewTransactionBuilder()
{
	return std::make_shared<ft3::TransactionBuilder>(shared_from_this());
}


std::shared_ptr<BlockchainSession> Blockchain::NewSession(std::shared_ptr<User> user)
{
	return std::make_shared<BlockchainSession>(user, shared_from_this());
}

void Blockchain::GetAccountsByParticipantId(string id, std::shared_ptr<User> user,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error)
{
	Account::GetByParticipantId(id, this->NewSession(user), on_success, on_error);
}

void Blockchain::GetAccountsByAuthDescriptorId(string id, std::shared_ptr<User> user,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error)
{
	Account::GetByAuthDescriptorId(id, this->NewSession(user), on_success, on_error);
}

void Blockchain::RegisterAccount(std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<User> user,
	std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{
	Account::Register(auth_descriptor, this->NewSession(user), on_success, on_error);
}

void Blockchain::GetAssetsByName(string name, std::function<void(std::vector<std::shared_ptr<Asset>>)> on_success, std::function<void(std::string)> on_error)
{
	Asset::GetByName(name, shared_from_this(), on_success, on_error);
}

void Blockchain::GetAssetById(string id, std::function<void(std::shared_ptr<Asset>)> on_success, std::function<void(std::string)> on_error)
{
	Asset::GetById(id, shared_from_this(), on_success, on_error);
}

void Blockchain::GetAllAssets(std::function<void(std::vector<std::shared_ptr<Asset>>)> on_success, std::function<void(std::string)> on_error)
{
	Asset::GetAssets(shared_from_this(), on_success, on_error);
}

void Blockchain::LinkChain(string chain_id, std::function<void(std::string)> on_success, std::function<void(std::string)> on_error) 
{
	std::shared_ptr<ArrayValue> op_args = AbstractValueFactory::EmptyArray();
	op_args->Add(AbstractValueFactory::Build(chain_id));

	std::shared_ptr<ft3::TransactionBuilder> tx_builder = this->NewTransactionBuilder();
	tx_builder->Add(ft3::Operation::Op("ft3.xc.link_chain", op_args));
	auto tx = tx_builder->Build({ {} }, on_error);
	tx->PostAndWait(on_success);
}

void Blockchain::IsLinkedWithChain(string chain_id, std::function<void(bool)> on_success, std::function<void(std::string)> on_error)
{
	this->Query("ft3.xc.is_linked_with_chain", {QueryObject("chain_rid", chain_id)}, 
		[on_success] (std::string is_linked) {
			on_success(is_linked.compare("1") == 0);	
		}, on_error);
}

void Blockchain::GetLinkedChainsIds(std::function<void(std::vector<std::string>)> on_success, std::function<void(std::string)> on_error)
{
	this->Query("ft3.xc.get_linked_chains", {},
		[on_success](std::string content) {
			//TO-DO
			on_success({ "" });
		}, on_error);
}

void Blockchain::GetLinkedChains(std::function<void(std::vector<std::shared_ptr<Blockchain>>)> on_success, std::function<void(std::string)> on_error)
{
	std::vector<std::string> chain_ids;
	this->GetLinkedChainsIds(
		[&chain_ids](std::vector<std::string> linked_chains) {
			for (std::string &id : linked_chains) {
				chain_ids.push_back(id);
			}
		}, on_error);

	std::vector<std::shared_ptr<Blockchain>> blockchains;
	for(std::string chaind_id : chain_ids)
	{
		Blockchain::Initialize(chaind_id, this->directory_service_,
			[&blockchains](std::shared_ptr<Blockchain> blockchain) {
				blockchains.push_back(blockchain);
		}, on_error);
	}

	on_success(blockchains);
}

void Blockchain::Query(string query_name, std::vector<QueryObject> query_objects, std::function<void(std::string)> on_success, std::function<void(std::string)> on_error)
{
	this->connection_->Query(query_name, query_objects, on_success, on_error);
}


void Blockchain::Call(std::shared_ptr<ft3::Operation> operation, std::shared_ptr<User> user,
	std::function<void(std::string)> on_success, std::function<void(string)> on_error)
{
	std::string rid = this->connection_->blockchain_rid_;

	auto tx_builder = this->NewTransactionBuilder();
	tx_builder->Add(operation);
	tx_builder->Add(AccountOperations::Nop());
	auto tx = tx_builder->Build(user->auth_descriptor_->Signers(), on_error);
	tx->Sign(user->key_pair_);
	tx->PostAndWait(on_success);

	rid = this->connection_->blockchain_rid_;
}

} // namespace ft3
} //namespace postchain
} //namespace chromia
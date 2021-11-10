#include "blockchain.h"
#include "blockchain_info.h"
#include "directory_service.h"
#include "../../Core/transaction_builder.h"

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

void Blockchain::Initialize(string blockchain_rid, DirectoryService directory_service,
	std::function<void(std::shared_ptr<BlockchainInfo>)> on_success, std::function<void(std::string)> on_error)
{
	std::shared_ptr<ChainConnectionInfo> chainConnectionInfo = directory_service.GetChainConnectionInfo(blockchain_rid);

	if (chainConnectionInfo == nullptr)
	{
		throw new std::exception("Cannot find details for chain"); // Add RID
	}

	std::shared_ptr<BlockchainClient> connection = std::make_shared<BlockchainClient>();
	connection->Setup(blockchain_rid, chainConnectionInfo->url_);
	BlockchainInfo::GetInfo(connection, on_success, on_error);
}

//void Blockchain::Query(std::string query_name, std::vector<QueryObject> query_objects,
//	std::function<void(std::string)> on_success, std::function<void(std::string)> on_error)
//{
//	//this->connection_->Query(query_name, query_objects, on_success, on_error);
//}

std::shared_ptr<TransactionBuilder> Blockchain::NewTransactionBuilder()
{
	std::shared_ptr<Blockchain> self_ptr(this);
	return std::make_shared<ft3::TransactionBuilder>(self_ptr);
}


std::shared_ptr<BlockchainSession> Blockchain::NewSession(std::shared_ptr<User> user)
{
	return std::make_shared<BlockchainSession>(user, std::shared_ptr<Blockchain>(this));
}

void Blockchain::GetAccountsByParticipantId(string id, std::shared_ptr<User> user,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error)
{
	//yield return Account.GetByParticipantId(id, this.NewSession(user), onSuccess, onError);
}

void Blockchain::GetAccountsByAuthDescriptorId(string id, std::shared_ptr<User> user,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error)
{
	//yield return Account.GetByAuthDescriptorId(id, this.NewSession(user), onSuccess, onError);
}

void Blockchain::RegisterAccount(std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<User> user,
	std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{
	//yield return Account.Register(authDescriptor, this.NewSession(user), onSuccess, onError);
}

void Blockchain::GetAssetsByName(string name, std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error)
{
	//yield return Asset.GetByName(name, this, onSuccess, onError);
}

void Blockchain::GetAssetById(string id, std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{
	//yield return Asset.GetById(id, this, onSuccess, onError);
}

void Blockchain::GetAllAssets(std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(std::string)> on_error)
{
	//yield return Asset.GetAssets(this, onSuccess, onError);
}

void Blockchain::LinkChain(string chainId, std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{
	/*yield return this.TransactionBuilder()
		.Add(Operation.Op("ft3.xc.link_chain", chainId))
		.Build(new byte[][]{ }, onError)
		.PostAndWait(onSuccess);*/
}

void Blockchain::IsLinkedWithChain(string chainId, std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{
	/*yield return this.Query<int>("ft3.xc.is_linked_with_chain",
		new (string, object)[] { ("chain_rid", chainId) },
		(int is_linked) = > onSuccess(is_linked == 1), onError);*/
}

void Blockchain::GetLinkedChainsIds(std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{
	//return this.Query<string[]>("ft3.xc.get_linked_chains", null, onSuccess, onError);
}

void Blockchain::GetLinkedChains(std::function<void(std::vector<std::shared_ptr<Blockchain>>)> on_success, std::function<void(std::string)> on_error)
{
	/*string[] chaindIds = null;
	yield return this.GetLinkedChainsIds(
		(string[] linkedChains) = > chaindIds = linkedChains, onError
	);

	List<Blockchain> blockchains = new List<Blockchain>();
	foreach(var chaindId in chaindIds)
	{
		yield return Blockchain.Initialize(chaindId, this._directoryService,
			(Blockchain blockchain) = > blockchains.Add(blockchain), onError);
	}

	onSuccess(blockchains);*/
}

void Query(string query_name, std::vector<QueryObject> query_objects, std::function<void(std::string)> on_success, std::function<void(std::string)> on_error)
{
//	return this.Connection.Query<T>(queryName, queryObject, onSuccess, onError);
}

void Blockchain::Query(string query_name, std::vector<QueryObject> query_objects, std::function<void(std::string)> on_success, std::function<void(std::string)> on_error)
{

}

void Blockchain::Call(std::shared_ptr<ft3::Operation> operation, std::shared_ptr<User> user,
	std::function<void()> on_success, std::function<void(string)> on_error)
{

}

//{
	/*std::shared_ptr<TransactionBuilder> tx_builder = TransactionBuilder();

	std::shared_ptr<ArrayValue> op_args;
	op_args->Add(AbstractValueFactory::Build(asset_id));
	op_args->Add(AbstractValueFactory::Build(account_id));
	op_args->Add(AbstractValueFactory::Build(amount));
	tx_builder->Add(ft3::Operation::Op("ft3.dev_give_balance", op_args));
	tx_builder->Add(AccountOperations::Nop());

	std::shared_ptr<ft3::Transaction> tx = tx_builder->Build(std::vector<std::vector<byte>>(), on_error);

	std::function<void()> on_success_wrapper = [on_success]() {
		on_success();
	};*/

	/*tx->PostAndWait(on_success_wrapper);

	return TransactionBuilder()
		.Add(operation)
		.Add(AccountOperations.Nop())
		.Build(user.AuthDescriptor.Signers.ToArray(), onError)
		.Sign(user.KeyPair)
		.PostAndWait(onSuccess);*/
		//}

} // namespace ft3
} //namespace postchain
} //namespace chromia
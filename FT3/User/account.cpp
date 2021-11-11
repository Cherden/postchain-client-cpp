#include "account.h"
#include "../Core/util.h"
#include "asset_balance.h"
#include "rate_limit.h"
#include "../Core/Blockchain/blockchain.h"
#include "../Core/Blockchain/blockchain_session.h"
#include "user.h"
#include "../Core/transaction_builder.h"
#include "../Core/transaction.h"
#include "account_dev_operations.h"
#include "account_operations.h"
#include "AuthDescriptor/auth_descriptor_factory.h"

namespace chromia {
namespace postchain {
namespace ft3 {

Flags::Flags(std::vector<FlagsType> flags)
{
	this->flag_list_ = flags;
}

bool Flags::HasFlag(FlagsType flag)
{
	return std::find(flag_list_.begin(), flag_list_.end(), flag) != flag_list_.end();
}

bool Flags::IsValid(FlagsType flag)
{
	return flag == FlagsType::eAccount || flag == FlagsType::eTransfer;
}

std::shared_ptr<ArrayValue> Flags::ToGTV()
{
	std::shared_ptr<ArrayValue> valid_flags;
	for (FlagsType &flag : this->flag_list_)
	{
		if (IsValid(flag))
		{
			std::string as_string = FT3Util::FlagTypeToString(flag);
			valid_flags->Add(AbstractValueFactory::Build(as_string));
		}
	}
	return valid_flags;
}

Account::Account(std::string id, std::vector<std::shared_ptr<AuthDescriptor>> auth_descriptors, std::shared_ptr<BlockchainSession> session)
{
	this->id_ = id;
	this->auth_descriptors_ = auth_descriptors;
	this->session_ = session;
}

std::string Account::GetID()
{
	return this->id_;
}

std::shared_ptr<Blockchain> Account::GetBlockchain()
{
	return this->session_->blockchain_;
}

void Account::GetByParticipantId(std::string id, std::shared_ptr<BlockchainSession> session,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error)
{
	std::function<void(std::string)> on_success_wrapper = [session, on_success, on_error](std::string content) {
		std::vector<std::string> account_ids;
		// TO-DO
		Account::GetByIds(account_ids, session, on_success, on_error);
	};

	session->Query("ft3.get_accounts_by_participant_id", {QueryObject("id", id)}, on_success_wrapper, on_error);
}

void Account::GetByAuthDescriptorId(std::string id, std::shared_ptr<BlockchainSession> session,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error)
{
	std::function<void(std::string)> on_success_wrapper = [session, on_success, on_error](std::string content) {
		std::vector<std::string> account_ids;
		// TO-DO
		Account::GetByIds(account_ids, session, on_success, on_error);
	};

	session->Query("ft3.get_accounts_by_auth_descriptor_id", { QueryObject("descriptor_id", id) }, on_success_wrapper, on_error);
}

void Account::Register(std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<BlockchainSession> session,
	std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{}


std::vector<byte> Account::RawTransactionRegister(std::shared_ptr<User> user, std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<Blockchain> blockchain)
{
	std::vector<std::vector<byte>> signers;

	for (auto &signer : user->auth_descriptor_->signers_)
	{
		signers.push_back(signer);
	}

	for (auto &signer : auth_descriptor->signers_)
	{
		signers.push_back(signer);
	}

	std::shared_ptr<TransactionBuilder> tx_builder = blockchain->NewTransactionBuilder();

	return tx_builder->Add(AccountDevOperations::Register(user->auth_descriptor_))
		->Add(AccountOperations::AddAuthDescriptor(user->auth_descriptor_->id_, user->auth_descriptor_->id_, auth_descriptor))
		->Build(signers, nullptr)
		->Sign(user->key_pair_)
		->Raw();
}


std::vector<byte> Account::RawTransactionAddAuthDescriptor(std::string account_id, std::shared_ptr<User> user,
	std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<Blockchain> blockchain)
{
	std::vector<std::vector<byte>> signers;

	for (auto &signer : user->auth_descriptor_->signers_)
	{
		signers.push_back(signer);
	}

	for (auto &signer : auth_descriptor->signers_)
	{
		signers.push_back(signer);
	}

	std::shared_ptr<TransactionBuilder> tx_builder = blockchain->NewTransactionBuilder();

	return tx_builder->Add(AccountOperations::AddAuthDescriptor(user->auth_descriptor_->id_, user->auth_descriptor_->id_, auth_descriptor))
		->Build(signers, nullptr)
		->Sign(user->key_pair_)
		->Raw();
}

void Account::GetByIds(std::vector<std::string> ids, std::shared_ptr<BlockchainSession> session,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error)
{
	// TO-DO sure that Account::GetById(...) is sync call;
	std::vector<std::shared_ptr<Account>> accounts;
	std::function<void(std::shared_ptr<Account>)> on_success_wrapper = [&accounts](std::shared_ptr<Account> account) {
		accounts.push_back(account);
	};

	for(auto &id : ids)
	{
		Account::GetById(id, session, on_success_wrapper, on_error);
	}

	on_success(accounts);
}

void Account::GetById(std::string id, std::shared_ptr<BlockchainSession> session, std::function<void(shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{
	// TO-DO sure that Account::GetById(...) is sync call;
	shared_ptr<Account> account;
	session->Query("ft3.get_account_by_id", { QueryObject("id", id) }, [&account, session](std::string lambda_id) {
		if (lambda_id.size() > 0) {
			account = std::make_shared<Account>(lambda_id, std::vector<std::shared_ptr<AuthDescriptor>>(), session);
		}
	}, on_error);

	account->Sync([on_success, account] () {
		on_success(account);
	}, on_error);
}


void Account::AddAuthDescriptor(std::shared_ptr<AuthDescriptor> auth_descriptor, std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	this->session_->Call(AccountOperations::AddAuthDescriptor(
		this->id_, this->session_->user_->auth_descriptor_->id_, auth_descriptor
	), [&]() {
		this->auth_descriptors_.push_back(auth_descriptor);
		on_success();
	}, on_error);
}


void Account::IsAuthDescriptorValid(std::string id, std::function<void(bool)> on_success, std::function<void(std::string)> on_error)
{
	this->session_->Query("ft3.is_auth_descriptor_valid",
		{ QueryObject("account_id", this->id_), QueryObject("auth_descriptor_id", PostchainUtil::HexStringToByteVector(id)) },
		[on_success](std::string content) {
		on_success(PostchainUtil::StringToBool(content));
		}, on_error);
}


void Account::DeleteAllAuthDescriptorsExclude(std::shared_ptr<AuthDescriptor> auth_descriptor, std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	this->session_->Call(AccountOperations::DeleteAllAuthDescriptorsExclude(this->id_, auth_descriptor->id_),
		[&]() {
			this->auth_descriptors_.clear();
			this->auth_descriptors_.push_back(auth_descriptor);
			on_success();
		}, on_error);
}

void Account::DeleteAuthDescriptor(std::shared_ptr<AuthDescriptor> auth_descriptor, std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	this->session_->Call(AccountOperations::DeleteAuthDescriptor(this->id_, this->session_->user_->auth_descriptor_->id_, auth_descriptor->id_),
        [&] () {
		auto to_remove = std::find(this->auth_descriptors_.begin(), this->auth_descriptors_.end(), auth_descriptor);
		if (to_remove != this->auth_descriptors_.end())
		{
			this->auth_descriptors_.erase(to_remove);
		}
            
        on_success();
    }, on_error);
}

void Account::Sync(std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	SyncAssets([]() {}, on_error);
	SyncAuthDescriptors([]() {}, on_error);
	SyncRateLimit([]() {}, on_error);
	on_success();
}

void Account::SyncAssets(std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	AssetBalance::GetByAccountId(this->id_, this->session_->blockchain_,
		[&] (std::vector<std::shared_ptr<AssetBalance>> balances) {
		this->assets_ = balances;
		on_success();
	}, on_error);
}

void Account::SyncAuthDescriptors(std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	std::vector<std::shared_ptr<AuthDescriptorFactory::AuthDescriptorQuery>> auth_descriptors;

	this->session_->Query("ft3.get_account_auth_descriptors", { QueryObject("id", this->id_) },
		[&auth_descriptors] (std::string) {
			//TO-DO auth_descriptors->push_back();
		}, on_error);

	std::shared_ptr<AuthDescriptorFactory> auth_descriptor_factory = std::make_shared<AuthDescriptorFactory>();
	std::vector<std::shared_ptr<AuthDescriptor>> auth_list;

	for(auto &auth_descriptor : auth_descriptors)
	{
		auth_list.push_back(
			authDescriptorFactory.Create(
				FT3Util::StringToAuthType((string) auth_descriptor->type),
				PostchainUtil::HexStringToByteVector((string) auth_descriptor->args)
			)
		);
	}

	//this.AuthDescriptor = authList;
	//onSuccess();
}
/*{
	AuthDescriptorFactory.AuthDescriptorQuery[] authDescriptors = null;

	yield return this.Session.Query<AuthDescriptorFactory.AuthDescriptorQuery[]>("ft3.get_account_auth_descriptors",
		new (string, object)[] { ("id", this.Id) },
		(AuthDescriptorFactory.AuthDescriptorQuery[] authQuery) =>
		{
			authDescriptors = authQuery;
		}, onError);

	var authDescriptorFactory = new AuthDescriptorFactory();
	List<AuthDescriptor> authList = new List<AuthDescriptor>();

	foreach (var authDescriptor in authDescriptors)
	{
		authList.Add(
			authDescriptorFactory.Create(
				Util.StringToAuthType((string)authDescriptor.type),
				Util.HexStringToBuffer((string)authDescriptor.args)
			)
		);
	}

	this.AuthDescriptor = authList;
	onSuccess();
}*/

void Account::SyncRateLimit(std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
/*{
	yield return RateLimit.GetByAccountRateLimit(this.Id, this.Session.Blockchain,
		(RateLimit rateLimit) =>
		{
			this.RateLimit = rateLimit;
			onSuccess();
		}, onError
	);
}*/

std::shared_ptr<AssetBalance> Account::GetAssetById(std::string id)
{
	return nullptr;
}
//{
//    return this.Assets.Find(assetBalance => assetBalance.Asset.Id.Equals(id));
//}

void Account::TransferInputsToOutputs(std::shared_ptr<AbstractValue> inputs, std::shared_ptr<AbstractValue> outputs,
	std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
/* {
		bool isTransferCompleted = false;

		yield return this.Session.Call(AccountOperations.Transfer(inputs, outputs),
			() => isTransferCompleted = true, onError);

		if (isTransferCompleted)
		{
			yield return this.SyncAssets(onSuccess, onError);
		}
	}*/

void Account::Transfer(std::string account_id, std::string asset_id, long amount,
	std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
//{
//    var input = new List<object>{
//        this.Id,
//        assetId,
//        this.Session.User.AuthDescriptor.ID,
//        amount,
//        new object[] {}
//    }.ToArray();

//    var output = new List<object>{
//        accountId,
//        assetId,
//        amount,
//        new object[] {}
//    }.ToArray();

//    yield return this.TransferInputsToOutputs(
//        new object[] { input },
//        new object[] { output },
//        onSuccess, onError
//    );
//}

void Account::BurnTokens(std::string assetId, long amount, std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
//{
	/* var input = new List<object>(){
		this.Id,
		assetId,
		this.Session.User.AuthDescriptor.Hash(),
		amount,
		new object[] {}
	}.ToArray();

	yield return this.TransferInputsToOutputs(
		new List<object>() { input }.ToArray(),
		new List<object>() { }.ToArray(),
		onSuccess, onError
	);*/
	// }

void Account::XcTransfer(std::string destination_chain_id, std::string destination_account_id, std::string assetId, long amount,
	std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
/*{
	yield return this.Session.Call(this.XcTransferOp(
		destinationChainId, destinationAccountId, assetId, amount),
		() =>
		{
			this.SyncAssets(onSuccess, onError);
		}, onError
	);
}*/

std::shared_ptr<ft3::Operation> Account::XcTransferOp(std::string destination_chain_id, std::string destinationAccountId, std::string asset_id, long amount)
{
	return nullptr;
}
/*{
	var source = new object[] {
		this.Id,
		assetId,
		this.Session.User.AuthDescriptor.ID,
		amount,
		new object[]{}
	};

	var target = new object[] {
		destinationAccountId,
		new object[]{}
	};

	var hops = new string[] {
		destinationChainId
	};

	return AccountOperations.XcTransfer(source, target, hops);
}*/

} // namespace http
} // namespace postchain
} // namespace chromi
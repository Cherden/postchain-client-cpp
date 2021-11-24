#include "account.h"
#include "../Core/util.h"
#include "asset_balance.h"
#include "asset.h"
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
	std::shared_ptr<ArrayValue> valid_flags = AbstractValueFactory::EmptyArray();
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
	std::string rid = session->blockchain_->connection_->blockchain_rid_;
	this->session_ = session;
	rid = this->session_->blockchain_->connection_->blockchain_rid_;
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
		nlohmann::json json_obj = nlohmann::json::parse(content);
		std::vector<std::string> account_ids;
		for (std::string id : json_obj)
		{
			//id = id.substr(1, id.size() - 2);
			account_ids.push_back(id);
		}
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
{
	bool call_ok = false;

	session->Call(
		AccountDevOperations::Register(auth_descriptor), 
		[&call_ok] (){
			call_ok = true;
		}
	, on_error);

	if (!call_ok) return;

	std::shared_ptr<Account> account = std::make_shared<Account>(
		PostchainUtil::ByteVectorToHexString(auth_descriptor->Hash()),
		std::vector<std::shared_ptr<AuthDescriptor>> { auth_descriptor },
		session);

	account->Sync([on_success, account]() { on_success(account); }, on_error);
}


std::vector<byte> Account::RawTransactionRegister(std::shared_ptr<User> user, std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<Blockchain> blockchain)
{
	std::vector<std::vector<byte>> signers;

	for (auto &signer : user->auth_descriptor_->Signers())
	{
		signers.push_back(signer);
	}

	for (auto &signer : auth_descriptor->Signers())
	{
		signers.push_back(signer);
	}

	std::shared_ptr<TransactionBuilder> tx_builder = blockchain->NewTransactionBuilder();

	return tx_builder->Add(AccountDevOperations::Register(user->auth_descriptor_))
		->Add(AccountOperations::AddAuthDescriptor(user->auth_descriptor_->ID(), user->auth_descriptor_->ID(), auth_descriptor))
		->Build(signers, nullptr)
		->Sign(user->key_pair_)
		->Raw();
}


std::vector<byte> Account::RawTransactionAddAuthDescriptor(std::string account_id, std::shared_ptr<User> user,
	std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<Blockchain> blockchain)
{
	std::vector<std::vector<byte>> signers;

	for (auto &signer : user->auth_descriptor_->Signers())
	{
		signers.push_back(signer);
	}

	for (auto &signer : auth_descriptor->Signers())
	{
		signers.push_back(signer);
	}

	std::shared_ptr<TransactionBuilder> tx_builder = blockchain->NewTransactionBuilder();

	return tx_builder->Add(AccountOperations::AddAuthDescriptor(user->auth_descriptor_->ID(), user->auth_descriptor_->ID(), auth_descriptor))
		->Build(signers, nullptr)
		->Sign(user->key_pair_)
		->Raw();
}

void Account::GetByIds(std::vector<std::string> ids, std::shared_ptr<BlockchainSession> session,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error)
{
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
	session->Query("ft3.get_account_by_id", { QueryObject("id", id) }, [&account, &on_error, session](std::string lambda_id) {
		if (lambda_id.size() > 0) 
		{
			lambda_id = lambda_id.substr(1, lambda_id.size() - 2);
			account = std::make_shared<Account>(lambda_id, std::vector<std::shared_ptr<AuthDescriptor>>(), session);
		}
		else 
		{
			on_error("Account::GetById failed. Received corrupted or missing account id");
		}
	}, on_error);

	account->Sync([on_success, account] () {
		on_success(account);
	}, on_error);
}


void Account::AddAuthDescriptor(std::shared_ptr<AuthDescriptor> auth_descriptor, std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	this->session_->Call(AccountOperations::AddAuthDescriptor(
		this->id_, this->session_->user_->auth_descriptor_->ID(), auth_descriptor
	), [&]() {
		this->auth_descriptors_.push_back(auth_descriptor);
		on_success();
	}, on_error);
}


void Account::IsAuthDescriptorValid(std::string id, std::function<void(bool)> on_success, std::function<void(std::string)> on_error)
{
	// TO-DO validate sync call;
	this->session_->Query("ft3.is_auth_descriptor_valid",
		{ QueryObject("account_id", this->id_), QueryObject("auth_descriptor_id", PostchainUtil::HexStringToByteVector(id)) },
		[on_success](std::string content) {
		on_success(PostchainUtil::StringToBool(content));
		}, on_error);
}


void Account::DeleteAllAuthDescriptorsExclude(std::shared_ptr<AuthDescriptor> auth_descriptor, std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	// TO-DO validate sync call;
	this->session_->Call(AccountOperations::DeleteAllAuthDescriptorsExclude(this->id_, auth_descriptor->ID()),
		[&]() {
			this->auth_descriptors_.clear();
			this->auth_descriptors_.push_back(auth_descriptor);
			on_success();
		}, on_error);
}

void Account::DeleteAuthDescriptor(std::shared_ptr<AuthDescriptor> auth_descriptor, std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	this->session_->Call(AccountOperations::DeleteAuthDescriptor(this->id_, this->session_->user_->auth_descriptor_->ID(), auth_descriptor->ID()),
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
	std::string rid = this->session_->blockchain_->connection_->blockchain_rid_;
	SyncAssets([]() {}, on_error);
	SyncAuthDescriptors([]() {}, on_error);
	SyncRateLimit([]() {}, on_error);
	on_success();
}

void Account::SyncAssets(std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	std::string rid = this->session_->blockchain_->connection_->blockchain_rid_;

	AssetBalance::GetByAccountId(this->id_, this->session_->blockchain_,
		[&] (std::vector<std::shared_ptr<AssetBalance>> balances) {
		this->assets_ = balances;
		on_success();
	}, on_error);
}

void Account::SyncAuthDescriptors(std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	std::vector<std::shared_ptr<AuthDescriptorFactory::AuthDescriptorQuery>> auth_descriptors;

	std::function<void(std::string)> on_success_wrapper = [&auth_descriptors, on_success, on_error](std::string content) {
		nlohmann::json json_obj = nlohmann::json::parse(content);
		for (auto & json_item : json_obj)
		{
			std::string id = PostchainUtil::GetSafeJSONString(json_item, "id");
			std::string type = PostchainUtil::GetSafeJSONString(json_item, "type");
			std::string args = PostchainUtil::GetSafeJSONString(json_item, "args");
			std::shared_ptr<AuthDescriptorFactory::AuthDescriptorQuery> new_query = std::make_shared<AuthDescriptorFactory::AuthDescriptorQuery>(id, type, args);
			auth_descriptors.push_back(new_query);
		}
	};

	this->session_->Query("ft3.get_account_auth_descriptors", { QueryObject("id", this->id_) },
		on_success_wrapper, on_error);

	std::shared_ptr<AuthDescriptorFactory> auth_descriptor_factory = std::make_shared<AuthDescriptorFactory>();
	std::vector<std::shared_ptr<AuthDescriptor>> auth_list;

	for(auto &auth_descriptor : auth_descriptors)
	{
		auth_list.push_back(
			auth_descriptor_factory->Create(
				FT3Util::StringToAuthType((string) auth_descriptor->type_),
				PostchainUtil::HexStringToByteVector((string) auth_descriptor->args_)
			)
		);
	}

	this->auth_descriptors_.clear();
	this->auth_descriptors_.insert(this->auth_descriptors_.begin(), auth_list.begin(), auth_list.end());

	on_success();
}

void Account::SyncRateLimit(std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	RateLimit::GetByAccountRateLimit(this->id_, this->session_->blockchain_,
		[&] (std::shared_ptr<RateLimit> rate_limit) {
			this->rate_limit_ = rate_limit; // TO-DO check previous rate_limit deallocation, check if destructor is called
			on_success();
		}, on_error
	);
}

std::shared_ptr<AssetBalance> Account::GetAssetById(std::string id)
{
	for (auto &asset_balance : this->assets_)
	{
		if (asset_balance->asset_->id_.compare(id) == 0)
		{
			return asset_balance;
		}
	}
	return nullptr;
}

void Account::TransferInputsToOutputs(std::shared_ptr<AbstractValue> inputs, std::shared_ptr<AbstractValue> outputs,
	std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	// TO-DO validate sync call;

	bool is_transfer_completed = false;

	this->session_->Call(AccountOperations::Transfer(inputs, outputs), 
		[&is_transfer_completed]() { is_transfer_completed = true; },
		on_error);

	if (is_transfer_completed)
	{
		this->SyncAssets(on_success, on_error);
	}
}


void Account::Transfer(std::string account_id, std::string asset_id, long amount,
	std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	std::shared_ptr<ArrayValue> input = AbstractValueFactory::EmptyArray();
	input->Add(AbstractValueFactory::Build(this->id_));
	input->Add(AbstractValueFactory::Build(asset_id));
	input->Add(AbstractValueFactory::Build(this->session_->user_->auth_descriptor_->ID()));
	input->Add(AbstractValueFactory::Build(amount));
	input->Add(AbstractValueFactory::EmptyArray());

	std::shared_ptr<ArrayValue> output = AbstractValueFactory::EmptyArray();
	input->Add(AbstractValueFactory::Build(account_id));
	input->Add(AbstractValueFactory::Build(asset_id));
	input->Add(AbstractValueFactory::Build(amount));
	input->Add(AbstractValueFactory::EmptyArray());

	this->TransferInputsToOutputs(
		AbstractValueFactory::Build({ input }),
		AbstractValueFactory::Build({ output }),  
		on_success, on_error
	);
}


void Account::BurnTokens(std::string asset_id, long amount, std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	std::shared_ptr<ArrayValue> input = AbstractValueFactory::EmptyArray();
	input->Add(AbstractValueFactory::Build(this->id_));
	input->Add(AbstractValueFactory::Build(asset_id));
	input->Add(AbstractValueFactory::Build(this->session_->user_->auth_descriptor_->ID()));
	input->Add(AbstractValueFactory::Build(amount));
	input->Add(AbstractValueFactory::EmptyArray());

	this->TransferInputsToOutputs(
		AbstractValueFactory::Build({ input }),
		AbstractValueFactory::EmptyArray(),
		on_success, on_error
	);
}


void Account::XcTransfer(std::string destination_chain_id, std::string destination_account_id, std::string asset_id, long amount,
	std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	std::shared_ptr<ft3::Operation> xc_op = this->XcTransferOp(destination_chain_id, destination_account_id, asset_id, amount);

	this->session_->Call(this->XcTransferOp(
		destination_chain_id, destination_account_id, asset_id, amount),
		[this, on_success, on_error] () {
			this->SyncAssets(on_success, on_error);
		}, on_error
	);
}


std::shared_ptr<ft3::Operation> Account::XcTransferOp(std::string destination_chain_id, std::string destination_account_id, std::string asset_id, long amount)
{
	std::shared_ptr<ArrayValue> source = AbstractValueFactory::EmptyArray();
	source->Add(AbstractValueFactory::Build(this->id_));
	source->Add(AbstractValueFactory::Build(asset_id));
	source->Add(AbstractValueFactory::Build(this->session_->user_->auth_descriptor_->ID()));
	source->Add(AbstractValueFactory::Build(amount));
	source->Add(AbstractValueFactory::EmptyArray());

	std::shared_ptr<ArrayValue> target = AbstractValueFactory::EmptyArray();
	source->Add(AbstractValueFactory::Build(destination_account_id));
	source->Add(AbstractValueFactory::EmptyArray());

	std::vector<std::string> hops = {destination_chain_id};

	return AccountOperations::XcTransfer(source, target, hops);
}

} // namespace http
} // namespace postchain
} // namespace chromi
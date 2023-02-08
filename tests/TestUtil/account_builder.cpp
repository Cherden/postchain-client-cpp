#include "account_builder.h"
#include "../../src/PostchainClient/common.h"
#include "../../src/PostchainClient/postchain_util.h"
#include "../../src/FT3/User/account.h"
#include "../../src/FT3/User/user.h"
#include "../../src/FT3/Core/key_pair.h"
#include "../../src/FT3/User/rate_limit.h"
#include "../../src/FT3/User/asset.h"
#include "../../src/FT3/User/account.h"
#include "../../src/FT3/User/asset_balance.h"
#include "../../src/FT3/Core/Blockchain/blockchain.h"
#include "../../src/FT3/User/AuthDescriptor/multi_signature_auth_descriptor.h"
#include "../../src/FT3/User/AuthDescriptor/single_signature_auth_descriptor.h"

void AccountBuilder::DefaultErrorHandler(std::string error)
{
	chromia::postchain::PostchainUtil::Log(error);
}

AccountBuilder::AccountBuilder(std::shared_ptr<Blockchain> blockchain, std::shared_ptr<User> user) :
	flags_({ FlagsType::eAccount, FlagsType::eTransfer })
{
	std::shared_ptr <User> new_user = user;
	if (user == nullptr)
	{
		new_user = TestUser::SingleSig();
	}

	this->blockchain_ = blockchain;
	this->participants_ = { new_user->key_pair_ };
	this->user_ = new_user;
}

std::shared_ptr<AccountBuilder> AccountBuilder::CreateAccountBuilder(std::shared_ptr<Blockchain> blockchain, std::shared_ptr<User> user)
{
	return std::make_shared<AccountBuilder>(blockchain, user);
}

std::shared_ptr<AccountBuilder> AccountBuilder::WithAuthFlags(std::vector<FlagsType> flags)
{
	this->flags_.clear();
	for (FlagsType flag : flags)
	{
		this->flags_.push_back(flag);
	}
	return shared_from_this();
}

std::shared_ptr<AccountBuilder> AccountBuilder::WithParticipants(std::vector<std::shared_ptr<KeyPair>> participants)
{
	this->participants_.clear();
	for (auto &item : participants)
	{
		this->participants_.push_back(item);
	}

	return shared_from_this();
}

std::shared_ptr<AccountBuilder> AccountBuilder::WithBalance(std::shared_ptr<Asset> asset, int balance)
{
	this->asset_ = asset;
	this->balance_ = balance;
	return shared_from_this();
}

std::shared_ptr<AccountBuilder> AccountBuilder::WithPoints(int points)
{
	this->points_ = points;
	return shared_from_this();
}

std::shared_ptr<AccountBuilder> AccountBuilder::WithRequiredSignatures(int count)
{
	this->required_signatures_count_ = count;
	return shared_from_this();
}

void AccountBuilder::Build(std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{
	if (on_error == nullptr) on_error = this->DefaultErrorHandler;

	std::shared_ptr<Account> account = nullptr;

	this->RegisterAccount([&account](std::shared_ptr<Account> _account) {
		account = _account;
	}, on_error);

	if (account == nullptr) 
	{
		on_error("AccountBuilder::Build() Error. RegisterAccount(...) failed");
		return;
	}

	if (account != nullptr)
	{
		this->AddBalanceIfNeeded(account, []() {}, on_error);
		this->AddPointsIfNeeded(account, [&account](std::shared_ptr<RateLimit> rate_limit) {
			account->rate_limit_ = rate_limit;
		}, on_error);
		on_success(account);
	}
}

void AccountBuilder::RegisterAccount(std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{
	Account::Register(
		this->GetAuthDescriptor(),
		this->blockchain_->NewSession(this->user_),
		on_success, on_error
	);
}

void AccountBuilder::AddBalanceIfNeeded(std::shared_ptr<Account> account, std::function<void()> on_success, std::function<void(std::string)> on_error)
{
	if (this->asset_ != nullptr && this->balance_ != -1)
	{
		return AssetBalance::GiveBalance(account->id_, this->asset_->id_,
			this->balance_, this->blockchain_, on_success, on_error);
	}
}

void AccountBuilder::AddPointsIfNeeded(std::shared_ptr<Account> account, std::function<void(std::shared_ptr<RateLimit>)> on_success, std::function<void(std::string)> on_error)
{
	if (this->points_ > 0)
	{
		return RateLimit::GivePoints(account->id_, this->points_, this->blockchain_, []() {}, on_error);
	}

	RateLimit::GetByAccountRateLimit(account->id_, this->blockchain_, on_success, on_error);
}

std::shared_ptr<AuthDescriptor> AccountBuilder::GetAuthDescriptor()
{
	if (this->required_signatures_count_ > this->participants_.size())
	{
		throw std::exception("Number of required signatures has to be less than number of participants");
	}

	std::vector<std::vector<BYTE>> participants;

	if (this->participants_.size() > 1)
	{
		for (auto participant : this->participants_)
		{
			participants.push_back(participant->pub_key_);
		}

		std::shared_ptr<MultiSignatureAuthDescriptor> multi = std::make_shared<MultiSignatureAuthDescriptor>(
			participants,
			this->required_signatures_count_,
			this->flags_,
			this->user_->auth_descriptor_->Rule()
			);

		return std::dynamic_pointer_cast<AuthDescriptor>(multi);
	}
	else
	{
		std::shared_ptr<SingleSignatureAuthDescriptor> multi = std::make_shared<SingleSignatureAuthDescriptor>(
			this->participants_[0]->pub_key_,
			this->flags_,
			this->user_->auth_descriptor_->Rule()
			);

		return std::dynamic_pointer_cast<AuthDescriptor>(multi);
	}
}

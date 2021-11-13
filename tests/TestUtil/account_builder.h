#pragma once 

//#ifdef FT3_TESTS

#include <string>
#include <vector>
#include "test_user.h"
#include "../../FT3/forward_declarations.h"
#include "../../FT3/User/rate_limit.h"
#include "../../FT3/User/asset.h"
#include "../../FT3/User/account.h"
#include "../../FT3/User/asset_balance.h"
#include "../../FT3/Core/Blockchain/blockchain.h"

using namespace chromia::postchain::ft3;

class AccountBuilder
{
public:
	AccountBuilder(std::shared_ptr<Blockchain> blockchain, std::shared_ptr<User> user);

	static std::shared_ptr<AccountBuilder> CreateAccountBuilder(std::shared_ptr<Blockchain> blockchain, std::shared_ptr<User> user = nullptr);

	std::shared_ptr<AccountBuilder> WithAuthFlags(std::vector<FlagsType> flags);

	std::shared_ptr<AccountBuilder> WithParticipants(std::vector<std::shared_ptr<KeyPair>> participants);

	std::shared_ptr<AccountBuilder> WithBalance(std::shared_ptr<Asset> asset, int balance);

	std::shared_ptr<AccountBuilder> WithPoints(int points);

	std::shared_ptr<AccountBuilder> WithRequiredSignatures(int count);

	void build(std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error = nullptr)
	{
		if (on_error == nullptr) on_error = this->DefaultErrorHandler;

		std::shared_ptr<Account> account = nullptr;

		this->RegisterAccount([&account](std::shared_ptr<Account> _account) {
			account = _account;
		}, on_error);

		if (account != nullptr)
		{
			this->AddBalanceIfNeeded(account, [] () { }, on_error);
			this->AddPointsIfNeeded(account, [&account] (std::shared_ptr<RateLimit> rate_limit) {
				account->rate_limit_ = rate_limit;
			}, on_error);

			on_success(account);
		}
	}

private:
	std::shared_ptr<Blockchain> blockchain_;
	std::shared_ptr<User> user_;
	int balance_ = -1;
	std::shared_ptr<Asset> asset_;
	std::vector<std::shared_ptr<KeyPair>> participants_ = { std::make_shared<KeyPair>() };
	int required_signatures_count_ = 1;
	std::vector<FlagsType> flags_ = { FlagsType::eAccount, FlagsType::eTransfer };
	int points_ = 0;

	void DefaultErrorHandler(std::string error);

	void RegisterAccount(std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
	{
		Account::Register(
			this->GetAuthDescriptor(),
			this->blockchain_->NewSession(this->user_),
			on_success, on_error
		);
	}

	void AddBalanceIfNeeded(std::shared_ptr<Account> account, std::function<void()> on_success, std::function<void(string)> on_error)
	{
		if (this->asset_ != nullptr && this->balance_ != -1)
		{
			return AssetBalance::GiveBalance(account->id_, this->asset_->id_,
				this->balance_, this->blockchain_, on_success, on_error);
		}
	}

	void AddPointsIfNeeded(std::shared_ptr<Account> account, std::function<void(std::shared_ptr<RateLimit>)> on_success, std::function<void(std::string)> on_error)
	{
		if (this->points_ > 0)
		{
			return RateLimit::GivePoints(account->id_, this->points_, this->blockchain_, [] () { }, on_error);
		}

		RateLimit::GetByAccountRateLimit(account->id_, this->blockchain_, on_success, on_error);
	}

	std::shared_ptr<AuthDescriptor> GetAuthDescriptor()
	{
		if (this->required_signatures_count_ > this->participants_.size())
		{
			throw std::exception("Number of required signatures has to be less than number of participants");
		}

		std::vector<std::vector<byte>> participants;

		if (this->participants_.size() > 1)
		{
			for(auto participant : this->participants_)
			{
				participants.push_back(participant->pub_key_);
			}

			std::shared_ptr<MultiSignatureAuthDescriptor> multi = std::make_shared<MultiSignatureAuthDescriptor>(
				participants,
				this->required_signatures_count_,
				this->flags_,
				this->user_->auth_descriptor_->rule_
			);

			return std::dynamic_pointer_cast<AuthDescriptor>(multi);
		}
		else
		{
			std::shared_ptr<SingleSignatureAuthDescriptor> multi = std::make_shared<SingleSignatureAuthDescriptor>(
				participants,
				this->participants_[0]->pub_key_,
				this->flags_,
				this->user_->auth_descriptor_->rule_
				);

			return std::dynamic_pointer_cast<AuthDescriptor>(multi);
		}
	}

};

//#endif // FT3_TESTS
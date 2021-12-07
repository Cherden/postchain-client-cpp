#include "rate_limit_test.h"
#include "../../src/FT3/User/AuthDescriptor/auth_descriptor_rule.h"
#include "../../src/FT3/User/asset.h"
#include "../../src/FT3/User/user.h"
#include "../../src/FT3/User/account.h"
#include "../../src/FT3/User/account_operations.h"
#include "../../src/FT3/Core/transaction_builder.h"
#include "../../src/FT3/Core/transaction.h"
#include "../../src/FT3/Core/Blockchain/blockchain_session.h"
#include "../../src/FT3/Core/Blockchain/blockchain_info.h"
#include "../../src/FT3/Core/Blockchain/rate_limit_info.h"
#include "../../src/FT3/User/rate_limit.h"
#include "../../src/FT3/Core/key_pair.h"
#include "../../src/PostchainClient/postchain_util.h"
#include "../TestUtil/blockchain_util.h"
#include "../TestUtil/test_user.h"
#include "../TestUtil/test_util.h"
#include "../TestUtil/account_builder.h"

void RateLimitTest::DefaultErrorHandler(std::string error)
{
	chromia::postchain::PostchainUtil::Log(error);
};


void RateLimitTest::SetupBlockchain()
{
	std::function<void(std::shared_ptr<Blockchain>)> on_success = [&](std::shared_ptr<Blockchain> blockchain) {
		this->blockchain_ = blockchain;
	};
	BlockchainUtil::GetDefaultBlockchain(on_success, this->DefaultErrorHandler);
	return;
}


void RateLimitTest::MakeRequests(std::shared_ptr<Account> account, int requests, std::function<void(std::string)> on_success)
{
	auto tx_builder = blockchain_->NewTransactionBuilder();
	std::vector<std::vector<byte>> signers;
	std::vector<std::shared_ptr<User>> users;

	for (auto &sig : account->session_->user_->auth_descriptor_->Signers())
	{
		signers.push_back(sig);
	}

	for (int i = 0; i < requests; i++)
	{
		std::shared_ptr<User> user = TestUser::SingleSig();
		for (auto &sig : user->auth_descriptor_->Signers())
		{
			signers.push_back(sig);
		}
		users.push_back(user);
		tx_builder->Add(AccountOperations::AddAuthDescriptor(account->id_, account->session_->user_->auth_descriptor_->ID(), user->auth_descriptor_));
	}

	std::shared_ptr<Transaction> tx = tx_builder->Build(signers, DefaultErrorHandler);
	tx->Sign(account->session_->user_->key_pair_);

	for (auto &user : users)
	{
		tx->Sign(user->key_pair_);
	}

	tx->PostAndWait(on_success);
}


bool RateLimitTest::RateLimitTestRun1()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<BlockchainInfo> info = nullptr;
	BlockchainInfo::GetInfo(blockchain_->connection_, [&info](std::shared_ptr<BlockchainInfo> _info) { info = _info; }, DefaultErrorHandler);

	if (info->rate_limit_info_.max_points_ != REQUEST_MAX_COUNT) return false;

	if (info->rate_limit_info_.recovery_time_ != RECOVERY_TIME) return false;

	return true;
}


bool RateLimitTest::RateLimitTestRun2()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<User> user = TestUser::SingleSig();

	auto tx_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	tx_builder->WithParticipants({ user->key_pair_ });

	std::shared_ptr<Account> account = nullptr;
	tx_builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; });

	account->Sync(EmptyCallback, DefaultErrorHandler);
	if (account->rate_limit_->points_ != 1) return false;

	return true;
}


bool RateLimitTest::RateLimitTestRun3()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<User> user = TestUser::SingleSig();

	auto tx_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	tx_builder->WithParticipants({ user->key_pair_ });

	std::shared_ptr<Account> account = nullptr;
	tx_builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; });
	if (account == nullptr) return false;

	PostchainUtil::SleepForMillis(20 * 1000); // wait for 20 seconds

	RateLimit::ExecFreeOperation(account->GetID(), blockchain_, EmptyCallback, DefaultErrorHandler); // used to make one block
	RateLimit::ExecFreeOperation(account->GetID(), blockchain_, EmptyCallback, DefaultErrorHandler); // used to calculate the last block's timestamp (previous block).

	// check the balance
	account->Sync(EmptyCallback, DefaultErrorHandler);
	if (account->rate_limit_->points_ != POINTS_AT_ACCOUNT_CREATION + 4) return false;  // 20 seconds / 5s recovery time

	return true;
}


bool RateLimitTest::RateLimitTestRun4()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<User> user = TestUser::SingleSig();

	auto builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	builder->WithParticipants({ user->key_pair_ });
	builder->WithPoints(4);

	std::shared_ptr<Account> account = nullptr;
	builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; });
	if (account == nullptr) return false;

	bool successful = false;
	MakeRequests(account, 4 + POINTS_AT_ACCOUNT_CREATION, [&successful](std::string content) {successful = true; });
	if (!successful) return false;

	account->Sync(EmptyCallback, DefaultErrorHandler);
	if (account->rate_limit_->points_ != 0) return false;

	return true;
}


bool RateLimitTest::RateLimitTestRun5()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<User> user = TestUser::SingleSig();

	auto tx_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	tx_builder->WithParticipants({ user->key_pair_ });
	tx_builder->WithPoints(4);

	std::shared_ptr<Account> account = nullptr;
	tx_builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; });
	if (account == nullptr) return false;

	bool successful = false;
	MakeRequests(account, 4 + POINTS_AT_ACCOUNT_CREATION, [&successful](std::string content) {successful = true; });
	if (!successful) return false;

	account->Sync(EmptyCallback, DefaultErrorHandler);

	successful = false;
	MakeRequests(account, 8, [&successful](std::string content) { successful = true; });
	if (successful) return false;

	return true;
}

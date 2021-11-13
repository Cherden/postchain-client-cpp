#pragma once 

//#ifdef FT3_TESTS

#include <string>
#include <vector>
#include "test_user.h"
#include "../../FT3/forward_declarations.h"
#include "../../FT3/forward_declarations.h"

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

	void Build(std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error = nullptr);

private:
	std::shared_ptr<Blockchain> blockchain_;
	std::shared_ptr<User> user_;
	int balance_ = -1;
	std::shared_ptr<Asset> asset_;
	std::vector<std::shared_ptr<KeyPair>> participants_ = { std::make_shared<KeyPair>() };
	int required_signatures_count_ = 1;
	std::vector<FlagsType> flags_;
	int points_ = 0;

	static void DefaultErrorHandler(std::string error);

	void RegisterAccount(std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error);

	void AddBalanceIfNeeded(std::shared_ptr<Account> account, std::function<void()> on_success, std::function<void(string)> on_error);

	void AddPointsIfNeeded(std::shared_ptr<Account> account, std::function<void(std::shared_ptr<RateLimit>)> on_success, std::function<void(std::string)> on_error);

	std::shared_ptr<AuthDescriptor> GetAuthDescriptor();

};

//#endif // FT3_TESTS
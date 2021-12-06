#pragma once

#include <memory>

#include "../PostchainClient/operation.h"

#include "../FT3/Core/Blockchain/blockchain.h"
#include "../FT3/Core/Blockchain/blockchain_session.h"
#include "../FT3/User/account.h"
#include "../FT3/User/user.h"
#include "../FT3/User/AuthDescriptor/auth_descriptor_factory.h"

#include "sso_store_local_storage.h"
#include "sso_store.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class SSO
{
public:
	static std::string vault_url_;

	std::shared_ptr<Blockchain> blockchain_;
	std::shared_ptr<SSOStore> store_;
	
	SSO(std::shared_ptr<Blockchain> blockchain, std::shared_ptr<SSOStore> store = nullptr);

	static std::string GetVaultUrl();

	static void SetVaultUrl(std::string url);

	struct AccUserPair
	{
	public:
		std::shared_ptr<Account> account;
		std::shared_ptr<User> user;
	};

private:

	void GetAccountAndUserByStoredIds(std::function<void(std::vector<AccUserPair>)> on_success, std::function<void(std::string)> on_error);

	void GetAccountAndUserByStoredId(std::shared_ptr<SavedSSOAccount> saved_account, std::function<void(AccUserPair)> on_success, std::function<void(std::string)> on_error);

public: 
	void AutoLogin(std::function<void(std::vector<AccUserPair>)> on_success, std::function<void(string)> on_error);

	void InitiateLogin(string success_url, string cancel_url);

	void FinalizeLogin(std::string tx, function<void(AccUserPair)> on_success, std::function<void(string)> on_error);

private:
	std::string GetAccountId(std::shared_ptr<Gtx> gtx);

};
} // namespace ft3
} // namespace postchain
} // namespace chromia
#pragma once

#include <memory>
#include "../FT3/Core/key_pair.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class SavedSSOAccount
{
public:
	std::string sso_account_id_;
	std::string sso_priv_key_;

	SavedSSOAccount(std::string account_id, std::string priv_key);
};

class SavedSSOData
{
public:
	std::string tmp_tx_;
	std::string sso_tmp_priv_key_;
	std::vector<std::shared_ptr<SavedSSOAccount>> accounts_;

	void AddAccountOrPrivKey(std::string account_id, std::string priv_key);

	void RemoveAccount(std::string account_id);
};

class SSOStore
{
	std::shared_ptr<KeyPair> tmp_key_pair_;
	std::vector<byte> tmp_priv_key_;
	std::string tmp_tx;

public:

	virtual ~SSOStore() = default;

	virtual std::shared_ptr<KeyPair> GetTmpKeyPair() = 0;
    
	virtual std::vector<byte> GetTmpPrivKey() = 0;
	virtual void SetTmpPrivKey(std::vector<byte> key) = 0;

	virtual std::string GetTmpTx() = 0;
	virtual void SetTmpTx(std::string str) = 0;

    virtual std::vector<std::shared_ptr<SavedSSOAccount>> GetAccounts() = 0;
	virtual void AddAccount(std::string account_id, std::string priv_key) = 0;
	virtual void RemoveAccount(std::string account_id) = 0;
	virtual void Load() = 0;
	virtual void Save() = 0;
	virtual void ClearTmp() = 0;
};

} // namespace ft3
} // namespace postchain
} // namespace chromia
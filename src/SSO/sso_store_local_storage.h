#pragma once

#include <memory>
#include "sso_store.h"
#include "file_manager.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class SSOStoreLocalStorage : public SSOStore
{


public:
	std::shared_ptr<SavedSSOData> sso_data_;

	SSOStoreLocalStorage();

	void Load();

	void Save();

	std::shared_ptr<KeyPair> GetTmpKeyPair() override;

	std::string GetTmpTx() override;

	void SetTmpTx(std::string value) override;

	std::vector<BYTE> GetTmpPrivKey() override;

	void SetTmpPrivKey(std::vector<BYTE> value) override;

	void ClearTmp() override;

	void AddAccount(std::string account_Id, std::string priv_key);

	void RemoveAccount(std::string accountId);

	std::vector<std::shared_ptr<SavedSSOAccount>> GetAccounts();

	static std::string GetTempFileName();

private:
	static const std::string STORAGEKEY;
	static const std::string FILENAME;
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
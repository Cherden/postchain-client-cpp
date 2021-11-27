#pragma once

#include <memory>
#include "sso_store.h"
#include "file_manager.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class SSOStoreLocalStorage : SSOStore
{


public:
	std::shared_ptr<SavedSSOData> sso_data_;

    SSOStoreLocalStorage()
    {
        Load();
    }

    void Load()
    {
        std::string result;
        FileManager::LoadFromFile(FILENAME, result);
        if (result.size() > 0)
        {
            //SSOData = JsonConvert.DeserializeObject<SavedSSOData>(result);
			// TO-DO
			sso_data_ = std::make_shared<SavedSSOData>();
		}
        else
        {
			sso_data_ = std::make_shared<SavedSSOData>();
        }
    }

    void Save()
    {
		// TO-DO
        //std::string data = JsonConvert.SerializeObject(SSOData, Formatting.Indented);
		std::string data;
		FileManager::WriteToFile(FILENAME, data);
    }

    std::shared_ptr<KeyPair> GetTmpKeyPair() override
    {
		std::string priv_key_string = sso_data_->sso_tmp_priv_key_;

        if (priv_key_string.size() == 0) return nullptr;

        return std::make_shared<KeyPair>(priv_key_string);
    }

	std::string GetTmpTx() override
	{
		return sso_data_->tmp_tx_;
	}

	void SetTmpTx(std::string value) override
	{
		sso_data_->tmp_tx_ = value;
	}

    std::vector<byte> GetTmpPrivKey() override
    {
		std::string priv_key_string = sso_data_->sso_tmp_priv_key_;
		if (priv_key_string.size() == 0) return {};
		return PostchainUtil::HexStringToByteVector(priv_key_string);
    }

	void SetTmpPrivKey(std::vector<byte> value) override
	{
		sso_data_->sso_tmp_priv_key_ = PostchainUtil::ByteVectorToHexString(value);
	}

    void ClearTmp() override
    {
		sso_data_->sso_tmp_priv_key_ = "";
        sso_data_->tmp_tx_ = "";
    }

    void AddAccount(std::string account_Id, std::string priv_key)
    {
        sso_data_->AddAccountOrPrivKey(account_Id, priv_key);
    }

    void RemoveAccount(std::string accountId)
    {
        sso_data_->RemoveAccount(accountId);
    }

    std::vector<std::shared_ptr<SavedSSOAccount>> GetAccounts()
    {
        return sso_data_->accounts_;
    }

private:
	const std::string STORAGEKEY = "SSO";
	const std::string FILENAME = STORAGEKEY + ".dat";
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
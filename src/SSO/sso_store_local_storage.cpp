#include "sso_store_local_storage.h"

namespace chromia {
namespace postchain {
namespace ft3 {

const std::string SSOStoreLocalStorage::STORAGEKEY = "SSO";

const std::string SSOStoreLocalStorage::FILENAME = STORAGEKEY + ".dat";

std::string SSOStoreLocalStorage::GetTempFileName()
{
	return SSOStoreLocalStorage::FILENAME;
}

SSOStoreLocalStorage::SSOStoreLocalStorage()
{
	ChromaFileManager::InitPersistentRoot();
	sso_data_ = std::make_shared<SavedSSOData>();
}


void SSOStoreLocalStorage::Load()
{
	std::string result = "";
	ChromaFileManager::LoadFromFile(FILENAME, result);
	if (result.size() > 0)
	{
		sso_data_ = SavedSSOData::Derialize(result);
	}
	else
	{
		sso_data_ = std::make_shared<SavedSSOData>();
	}
}


void SSOStoreLocalStorage::Save()
{
	std::string data = SavedSSOData::Serialize(sso_data_);
	ChromaFileManager::WriteToFile(FILENAME, data);
}


std::shared_ptr<KeyPair> SSOStoreLocalStorage::GetTmpKeyPair()
{
	std::string priv_key_string = sso_data_->sso_tmp_priv_key_;

	if (priv_key_string.size() == 0) return nullptr;

	return std::make_shared<KeyPair>(priv_key_string);
}


std::string SSOStoreLocalStorage::GetTmpTx()
{
	return sso_data_->tmp_tx_;
}


void SSOStoreLocalStorage::SetTmpTx(std::string value)
{
	sso_data_->tmp_tx_ = value;
}


std::vector<BYTE> SSOStoreLocalStorage::GetTmpPrivKey()
{
	std::string priv_key_string = sso_data_->sso_tmp_priv_key_;
	if (priv_key_string.size() == 0) return {};
	return PostchainUtil::HexStringToByteVector(priv_key_string);
}


void SSOStoreLocalStorage::SetTmpPrivKey(std::vector<BYTE> value)
{
	sso_data_->sso_tmp_priv_key_ = PostchainUtil::ByteVectorToHexString(value);
}


void SSOStoreLocalStorage::ClearTmp()
{
	sso_data_->sso_tmp_priv_key_ = "";
	sso_data_->tmp_tx_ = "";
}


void SSOStoreLocalStorage::AddAccount(std::string account_Id, std::string priv_key)
{
	sso_data_->AddAccountOrPrivKey(account_Id, priv_key);
}


void SSOStoreLocalStorage::RemoveAccount(std::string accountId)
{
	sso_data_->RemoveAccount(accountId);
}


std::vector<std::shared_ptr<SavedSSOAccount>> SSOStoreLocalStorage::GetAccounts()
{
	return sso_data_->accounts_;
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
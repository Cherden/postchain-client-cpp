#include "sso_store.h"

namespace chromia {
namespace postchain {
namespace ft3 {

void SavedSSOData::AddAccountOrPrivKey(std::string account_id, std::string priv_key)
{
	std::shared_ptr<SavedSSOAccount> result = nullptr;

	for (auto item : accounts_)
	{
		if (item->sso_account_id_.compare(account_id) == 0)
		{
			result = item;
			break;
		}
	}

	if (result != nullptr)
	{
		result->sso_priv_key_ = priv_key;
	}
	else
	{
		accounts_.push_back(std::make_shared<SavedSSOAccount>(account_id, priv_key));
	}
}


void SavedSSOData::RemoveAccount(std::string account_id)
{
	std::shared_ptr<SavedSSOAccount> result = nullptr;
	for (int i = 0; i < accounts_.size(); i++)
	{
		if (accounts_[i]->sso_account_id_.compare(account_id) == 0)
		{
			accounts_.erase(accounts_.begin() + i);
		}
	}
}


std::string SavedSSOData::Serialize(std::shared_ptr<SavedSSOData> data)
{
	nlohmann::json json_obj;
	json_obj["__tmpTX"] = data->tmp_tx_;
	json_obj["__ssoTmpPrivKey"] = data->sso_tmp_priv_key_;
	json_obj["__accounts"] = {};

	for (auto &account : data->accounts_)
	{
		nlohmann::json json_acc;
		json_acc["__ssoAccountId"] = account->sso_account_id_;
		json_acc["__ssoPrivKey"] = account->sso_priv_key_;
	}

	return json_obj.dump();
}


std::shared_ptr<SavedSSOData> SavedSSOData::Derialize(std::string data)
{
	std::shared_ptr<SavedSSOData> res = std::make_shared<SavedSSOData>();

	nlohmann::json json_obj = nlohmann::json::parse(data);
	res->sso_tmp_priv_key_ = json_obj["__ssoTmpPrivKey"];
	res->tmp_tx_ = json_obj["__tmpTX"];
	
	for (auto &json_acc : json_obj["__accounts"])
	{
		std::string sso_account_id_ = json_acc["__ssoAccountId"];
		std::string sso_priv_key_ = json_acc["__ssoPrivKey"];
		std::shared_ptr<SavedSSOAccount> acc = std::make_shared<SavedSSOAccount>(sso_account_id_, sso_priv_key_);
		res->accounts_.push_back(acc);
	}

	return res;
}


SavedSSOAccount::SavedSSOAccount(std::string account_id, std::string priv_key)
{
	sso_account_id_ = account_id;
	sso_priv_key_ = priv_key;
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
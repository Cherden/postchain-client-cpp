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


SavedSSOAccount::SavedSSOAccount(std::string account_id, std::string priv_key)
{
	sso_account_id_ = account_id;
	sso_priv_key_ = priv_key;
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
#include "sso.h"
#include "../src/operation.h"
#include "../src/GTX/gtx_value.h"

// TO-DO get rid of UE4 dependencies
#include "../../ChromaUnreal/Utils.h"
#include "Kismet/KismetSystemLibrary.h"
using namespace chromia::postchain;

namespace chromia {
namespace postchain {
namespace ft3 {

string SSO::vault_url_ = "https://vault-testnet.chromia.com";


std::string SSO::GetVaultUrl()
{
	return SSO::vault_url_;
}


void SSO::SetVaultUrl(std::string url)
{
	SSO::vault_url_ = url;
}


SSO::SSO(std::shared_ptr<Blockchain> blockchain, std::shared_ptr<SSOStore> store)
{
	this->blockchain_ = blockchain;

	if (store == nullptr)
	{
		store_ = std::make_shared<SSOStoreLocalStorage>();
	}
	else
	{
		store_ = store;
	}
}


void SSO::GetAccountAndUserByStoredIds(std::function<void(std::vector<AccUserPair>)> on_success, std::function<void(std::string)> on_error)
{
	std::vector<AccUserPair> aus;
	std::vector<std::shared_ptr<SavedSSOAccount>> accounts = this->store_->GetAccounts();

	for (auto acc : accounts)
	{
		GetAccountAndUserByStoredId(acc, [&aus](AccUserPair au) { aus.push_back(au); }, on_error);
	}

	on_success(aus);
}


void SSO::GetAccountAndUserByStoredId(std::shared_ptr<SavedSSOAccount> saved_account, std::function<void(AccUserPair)> on_success, std::function<void(std::string)> on_error)
{
	auto key_pair = std::make_shared<KeyPair>(saved_account->sso_priv_key_);
	auto auth_descriptor = std::make_shared<SingleSignatureAuthDescriptor>(key_pair->pub_key_, std::vector<FlagsType>{ FlagsType::eTransfer });
	auto user = std::make_shared<User>(key_pair, auth_descriptor);

	std::shared_ptr<Account> account = nullptr;

	this->blockchain_->NewSession(user)->GetAccountById(saved_account->sso_account_id_,
		[&account](std::shared_ptr<Account> _account) { account = _account; }, on_error);

	if (account != nullptr)
	{
		account->IsAuthDescriptorValid(user->auth_descriptor_->ID(),
			[on_success, account, user](bool is_valid)
		{
			AccUserPair pair;
			pair.account = account;
			pair.user = user;
			if (is_valid) on_success(pair);
		},
		on_error // delete authdescriptor from localstorage?
		);
	}
}


void SSO::AutoLogin(std::function<void(std::vector<AccUserPair>)> on_success, std::function<void(string)> on_error)
{
	//GetAccountAndUserByStoredIds(on_success, on_error);
}


void SSO::InitiateLogin(string success_url, string cancel_url)
{
	std::shared_ptr<KeyPair> key_pair = std::make_shared<KeyPair>();
	this->store_->SetTmpPrivKey(key_pair->priv_key_);
	this->store_->Save();

	FString url = "";
	url += ChromaUtils::STDStringToFString(SSO::vault_url_);
	url += "/?route=/authorize&dappId=";
	url += ChromaUtils::STDStringToFString(this->blockchain_->id_);
	url += "&pubkey=";
	url += ChromaUtils::STDStringToFString(PostchainUtil::ByteVectorToHexString(key_pair->pub_key_));
	url += "&successAction=";
	url += ChromaUtils::STDStringToFString(success_url);
	url += "&cancelAction=";
	url += ChromaUtils::STDStringToFString(cancel_url);
	url += "&version=0.1";

	UKismetSystemLibrary::LaunchURL(url);
}


void SSO::FinalizeLogin(std::string tx, function<void(AccUserPair)> on_success, std::function<void(string)> on_error)
{
	auto key_pair = this->store_->GetTmpKeyPair();
	this->store_->ClearTmp();

	if (key_pair == nullptr)
	{
		throw std::exception("Error loading public key");
	}

	auto auth_descriptor = std::make_shared<SingleSignatureAuthDescriptor>(key_pair->pub_key_, std::vector<FlagsType> { FlagsType::eTransfer });
	auto user = std::make_shared<User>(key_pair, auth_descriptor);

	auto gtx = PostchainUtil::DeserializeGTX(PostchainUtil::HexStringToByteVector(tx));
	gtx->Sign(key_pair->priv_key_, key_pair->pub_key_);

	auto connection = this->blockchain_->connection_;
	auto postchain_transaction = std::make_shared<PostchainTransaction>(gtx, connection->base_url_, connection->blockchain_rid_, on_error);

	bool is_success = false;
	postchain_transaction->PostAndWait([&is_success](std::string content) {is_success = true; });

	if (is_success)
	{
		auto account_id = GetAccountId(gtx);
		this->store_->AddAccount(account_id, PostchainUtil::ByteVectorToHexString(key_pair->priv_key_));
		this->store_->Save();

		this->blockchain_->NewSession(user)->GetAccountById(account_id,
			[on_success, user](std::shared_ptr<Account> _account) {
				AccUserPair pair;
				pair.account = _account;
				pair.user = user;
				on_success(pair);
			},
			on_error);
	}
}


std::string SSO::GetAccountId(std::shared_ptr<Gtx> gtx)
{
	auto ops = gtx->operations_;
	if (ops.size() == 1)
	{
		return ops[0]->GetArgs()[0]->string_;
	}
	else if (ops.size() == 2)
	{
		return ops[1]->GetArgs()[0]->string_;
	}
	else
	{
		throw std::exception("Invalid sso transaction");
	}
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
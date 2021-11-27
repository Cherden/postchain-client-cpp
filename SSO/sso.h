#pragma once

#include <memory>

#include "../src/operation.h"

#include "../FT3/Core/Blockchain/blockchain.h"
#include "../FT3/Core/Blockchain/blockchain_session.h"
#include "../FT3/User/account.h"
#include "../FT3/User/user.h"
#include "../FT3/User/AuthDescriptor/auth_descriptor_factory.h"

#include "sso_store_local_storage.h"
#include "sso_store.h"

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformProcess.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class SSO
{
private:
	static std::string vault_url_;

public:
	std::shared_ptr<Blockchain> blockchain_;
	std::shared_ptr<SSOStore> store_;
	
	SSO(std::shared_ptr<Blockchain> blockchain, std::shared_ptr<SSOStore> store = nullptr)
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

	static std::string GetVaultUrl();

	static void SetVaultUrl(std::string url);

private:

	struct AccUserPair
	{
	public:
		std::shared_ptr<Account> account;
		std::shared_ptr<User> user;
	};

	void GetAccountAndUserByStoredIds(std::function<void(std::vector<AccUserPair>)> on_success, std::function<void(std::string)> on_error)
	{
		std::vector<AccUserPair> aus;
		std::vector<std::shared_ptr<SavedSSOAccount>> accounts = this->store_->GetAccounts();

		for(auto acc : accounts)
		{
			GetAccountAndUserByStoredId(acc, [&aus] (AccUserPair au) { aus.push_back(au); }, on_error);
		}

		on_success(aus);
	}

	void GetAccountAndUserByStoredId(std::shared_ptr<SavedSSOAccount> saved_account, std::function<void(AccUserPair)> on_success, std::function<void(std::string)> on_error)
	{
		auto key_pair = std::make_shared<KeyPair>(saved_account->sso_priv_key_);
		auto auth_descriptor = std::make_shared<SingleSignatureAuthDescriptor>(key_pair->pub_key_, std::vector<FlagsType> { FlagsType::eTransfer });
		auto user = std::make_shared<User>(key_pair, auth_descriptor);

		std::shared_ptr<Account> account = nullptr;

		this->blockchain_->NewSession(user)->GetAccountById(saved_account->sso_account_id_,
			[&account](std::shared_ptr<Account> _account) { account = _account; }, on_error);

		if (account != nullptr)
		{
			account->IsAuthDescriptorValid(user->auth_descriptor_->ID(),
				[on_success, account, user] (bool is_valid)
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

public: 
	void AutoLogin(std::function<void(std::vector<AccUserPair>)> on_success, std::function<void(string)> on_error)
	{
		GetAccountAndUserByStoredIds(on_success, on_error);
	}

	void InitiateLogin(string success_url, string cancel_url)
	{
		std::shared_ptr<KeyPair> key_pair = std::make_shared<KeyPair>();
		this->store_->SetTmpPrivKey(key_pair->priv_key_);
		this->store_->Save();

		FString url = "";
		url += SSO::vault_url_;
		url += "/?route=/authorize&dappId=";
		url += this->blockchain_->id_;
		url += "&pubkey=";
		url += PostchainUtil::ByteVectorToHexString(key_pair->pub_key_);
		url += "&successAction=";
		url += success_url;
		url += "&cancelAction=";
		url += cancel_url;
		url += "&version=0.1";

		FGenericPlatformProcess::LaunchURL(*url, NULL, NULL);
	}

	void FinalizeLogin(std::string tx, function<void(AccUserPair)> on_success, std::function<void(string)> on_error)
	{
		/*var keyPair = this.Store.TmpKeyPair;
		this.Store.ClearTmp();

		if (keyPair == null) throw new Exception("Error loading public key");

		var authDescriptor = new SingleSignatureAuthDescriptor(keyPair.PubKey, new FlagsType[]{ FlagsType.Transfer });
		var user = new User(keyPair, authDescriptor);

		var gtx = PostchainUtil.DeserializeGTX(Util.HexStringToBuffer(tx));
		gtx.Sign(keyPair.PrivKey, keyPair.PubKey);

		var connection = this.Blockchain.Connection;
		var postchainTransaction = new PostchainTransaction(gtx, connection.BaseUrl, connection.BlockchainRID, onError);

		bool isSuccess = false;
		yield return postchainTransaction.PostAndWait(() = > isSuccess = true);

		if (isSuccess)
		{
			var accountID = GetAccountId(gtx);
			this.Store.AddAccount(accountID, Util.ByteArrayToString(keyPair.PrivKey));
			this.Store.Save();

			yield return this.Blockchain.NewSession(user).GetAccountById(accountID,
				(Account _account) = > onSuccess((_account, user)), onError);
		}*/
	}

private:
	std::string GetAccountId(std::shared_ptr<Gtx> gtx)
	{
		/*auto ops = gtx->operations_;
		if (ops.size() == 1)
		{
			return ops[0]->args_[0]->string_;
		}
		else if (ops.Count == 2)
		{
			return ops[1].Args[0].String;
		}
		else
		{
			throw new Exception("Invalid sso transaction");
		}*/
	}
//
//		public IEnumerator Logout<T>((Account, User) au, Action onSuccess, Action<string> onError)
//		{
//			yield return au.Item1.DeleteAuthDescriptor(au.Item2.AuthDescriptor, () = >
//			{
//				this.Store.ClearTmp();
//				this.Store.Save();
//				onSuccess();
//			}, onError);
//		}
//
//		public static Dictionary<string, string> GetParams(string uri)
//		{
//			var matches = Regex.Matches(uri, @"[\ ? &](([^&=] + ) = ([^ &= #] * ))", RegexOptions.Compiled);
//				return matches.Cast<Match>().ToDictionary(
//					m = > Uri.UnescapeDataString(m.Groups[2].Value),
//					m = > Uri.UnescapeDataString(m.Groups[3].Value)
//				);
//		}
//
//		// For Webgl
//		public IEnumerator PendingSSO(Action<(Account, User)> onSuccess, Action<string> onError)
//		{
//			var url = UnityEngine.Application.absoluteURL;
//			var pairs = GetParams(url);
//
//			if (pairs.ContainsKey("rawTx"))
//			{
//				var raw = pairs["rawTx"];
//				yield return FinalizeLogin(raw, onSuccess, onError);
//			}
//		}


};
} // namespace ft3
} // namespace postchain
} // namespace chromia
#pragma once 

//#ifdef FT3_TESTS

//#include <gtest/gtest.h>

//#include "GoogleTest/include/gtest/gtest.h"

#include "CoreMinimal.h" // TO-DO get rid of UE4 dependencies

#include <functional>
#include <string>

#include "../../src/common.h"

#include "../TestUtil/blockchain_util.h"
#include "../TestUtil/test_user.h"
#include "../TestUtil/account_builder.h"
#include "../TestUtil/test_operators.h"

#include "../../src/postchain_util.h"

#include "../../FT3/User/account.h"
#include "../../FT3/User/user.h"
#include "../../FT3/User/account_operations.h"
#include "../../FT3/Core/Blockchain/blockchain.h"
#include "../../FT3/Core/transaction_builder.h"
#include "../../FT3/Core/transaction.h"
#include "../../../ChromaUnreal/Utils.h"


using namespace chromia::postchain::ft3;
using namespace chromia::postchain;

class AccountTest
{
private:
	std::shared_ptr<Blockchain> blockchain_;

	std::function<void(std::string)> default_error_handler_ = [](std::string error) {
		UE_LOG(LogTemp, Error, TEXT("CHROMA::FT3 [%s]"), *(ChromaUtils::STDStringToFString(error)));
	};


	void SetupBlockchain()
	{
		std::function<void(std::shared_ptr<Blockchain>)> on_success = [&](std::shared_ptr<Blockchain> blockchain) {
			this->blockchain_ = blockchain;
		};
		
		std::function<void(std::string)> on_error = [] (std::string error) {
			UE_LOG(LogTemp, Display, TEXT("FT3::Error. SetupBlockchain()"));
		};

		BlockchainUtil::GetDefaultBlockchain(on_success, on_error);
	}

	
	void EmptyCallback() {}

	
	void AddAuthDescriptorTo(std::shared_ptr<Account> account, std::shared_ptr<User> admin_user, std::shared_ptr<User> user, std::function<void()> on_success)
	{
		vector<vector<byte>> signers;
		for (auto &signer : admin_user->auth_descriptor_->signers_)
		{
			signers.push_back(signer);
		}
		for (auto &signer : user->auth_descriptor_->signers_)
		{
			signers.push_back(signer);
		}
		
		auto tx_builder = blockchain_->NewTransactionBuilder();
		tx_builder->Add(AccountOperations::AddAuthDescriptor(account->id_, admin_user->auth_descriptor_->id_, user->auth_descriptor_));

		auto tx = tx_builder->Build(signers, this->default_error_handler_);
		tx->Sign(admin_user->key_pair_);
		tx->Sign(user->key_pair_);
		tx->PostAndWait(on_success);
	}

public:

	// Correctly creates keypair
	bool AccountTest1()
	{
		bool pass = true;

		std::vector<unsigned char> private_key;
		std::vector<unsigned char> public_key;
		if (!PostchainUtil::GenerateKeyPair(private_key, public_key))
		{
			pass = false;
		}

		auto user = new KeyPair(PostchainUtil::ByteVectorToHexString(private_key));

		pass = pass && TestOperators::Equals(user->priv_key_, private_key);
		pass = pass && TestOperators::Equals(user->pub_key_, public_key);

		return pass;
	}

	// Register account on blockchain
	bool AccountTest2()
	{
		bool pass = true;

		SetupBlockchain();
		std::shared_ptr<User> user = TestUser::SingleSig();
		std::shared_ptr<Account> account = nullptr;
		blockchain_->RegisterAccount(user->auth_descriptor_, user, [&account](std::shared_ptr<Account> _account) {
			account = _account;
		}, this->default_error_handler_);

		pass = pass && TestOperators::NotNull(account);
		return pass;
	}

	// can add new auth descriptor if has account edit rights
	bool AccountTest3()
	{
		SetupBlockchain();
		std::shared_ptr<User> user = TestUser::SingleSig();

		std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
		/*accountBuilder.WithParticipants(new List<KeyPair>() { user.KeyPair });
		accountBuilder.WithPoints(1);

		Account account = null;
		yield return accountBuilder.Build((Account _account) = > { account = _account; }, DefaultErrorHandler);
		Assert.NotNull(account);

		yield return account.AddAuthDescriptor(
			new SingleSignatureAuthDescriptor(
				user.KeyPair.PubKey,
				new List<FlagsType>() { FlagsType.Transfer }.ToArray()
			),
			EmptyCallback, DefaultErrorHandler
		);

		Assert.AreEqual(2, account.AuthDescriptor.Count);*/
	}

};

//#endif // FT3_TESTS
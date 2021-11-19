#include "account_test.h"

#include "CoreMinimal.h" // TO-DO get rid of UE4 dependencies


void AccountTest::DefaultErrorHandler(std::string error) 
{
	UE_LOG(LogTemp, Error, TEXT("CHROMA::Error [%s]"), *(ChromaUtils::STDStringToFString(error)));
};


void AccountTest::SetupBlockchain()
{
	std::function<void(std::shared_ptr<Blockchain>)> on_success = [&](std::shared_ptr<Blockchain> blockchain) {
		this->blockchain_ = blockchain;
	};
	BlockchainUtil::GetDefaultBlockchain(on_success, this->DefaultErrorHandler);
	return;
}


void AccountTest::AddAuthDescriptorTo(std::shared_ptr<Account> account, std::shared_ptr<User> admin_user, std::shared_ptr<User> user, std::function<void()> on_success)
{
	vector<vector<byte>> signers;
	for (auto &signer : admin_user->auth_descriptor_->Signers())
	{
		signers.push_back(signer);
	}
	for (auto &signer : user->auth_descriptor_->Signers())
	{
		signers.push_back(signer);
	}

	auto tx_builder = blockchain_->NewTransactionBuilder();
	tx_builder->Add(AccountOperations::AddAuthDescriptor(account->id_, admin_user->auth_descriptor_->ID(), user->auth_descriptor_));

	auto tx = tx_builder->Build(signers, this->DefaultErrorHandler);
	tx->Sign(admin_user->key_pair_);
	tx->Sign(user->key_pair_);

	std::function<void(std::string)> on_success_wrapper = [on_success](std::string content) {
		on_success();
	};

	tx->PostAndWait(on_success_wrapper);
}

// Correctly creates keypair
bool AccountTest::AccountTest1()
{
	std::vector<unsigned char> private_key;
	std::vector<unsigned char> public_key;
	if (!PostchainUtil::GenerateKeyPair(private_key, public_key))
	{
		return false;
	}

	auto user = new KeyPair(PostchainUtil::ByteVectorToHexString(private_key));

	if (!TestOperators::Equals(user->priv_key_, private_key)) return false;
	if (!TestOperators::Equals(user->pub_key_, public_key)) return false;

	return true;
}

// Register account on blockchain
bool AccountTest::AccountTest2()
{
	SetupBlockchain();
	if (blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<User> user = TestUser::SingleSig();
	std::shared_ptr<Account> account = nullptr;
	blockchain_->RegisterAccount(user->auth_descriptor_, user, [&account](std::shared_ptr<Account> _account) {
		account = _account;
	}, this->DefaultErrorHandler);

	if(!account) return false;
	return true;
}

// Can add new auth descriptor if has account edit rights
bool AccountTest::AccountTest3()
{

	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	auto user = TestUser::SingleSig();
	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	account_builder->WithParticipants(std::vector<std::shared_ptr<KeyPair>> { user->key_pair_ });
	account_builder->WithPoints(1);

	std::shared_ptr<Account> account = nullptr;
	account_builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; }, this->DefaultErrorHandler);

	if (!account) return false;

	std::vector<FlagsType> flags = { FlagsType::eTransfer };

	auto single_sig = std::make_shared<SingleSignatureAuthDescriptor>(
		user->key_pair_->pub_key_,
		std::vector<FlagsType>{ FlagsType::eTransfer });

	account->AddAuthDescriptor(
		single_sig,
		this->EmptyCallback,
		this->DefaultErrorHandler
	);

	if (!TestOperators::Equals(2, account->auth_descriptors_.size())) return false;
	return true;
}

// Cannot add new auth descriptor if account doesn't have account edit rights
bool AccountTest::AccountTest4()
{
	this->SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<User> user = TestUser::SingleSig();
	std::shared_ptr<Account> account = nullptr;

	Account::Register(
		std::make_shared<SingleSignatureAuthDescriptor>(user->key_pair_->pub_key_, std::vector<FlagsType>{ FlagsType::eTransfer }),
		this->blockchain_->NewSession(user),
		[&account](std::shared_ptr<Account> _account) { account = _account; },
		this->DefaultErrorHandler);

	if(account == nullptr) return false;

	account->AddAuthDescriptor(
		std::make_shared<SingleSignatureAuthDescriptor>(user->key_pair_->pub_key_, std::vector<FlagsType>{ FlagsType::eTransfer }),
		this->EmptyCallback, this->DefaultErrorHandler);

	if (!TestOperators::Equals(1, account->auth_descriptors_.size())) return false;

	return true;
}


// Should create new multisig account
bool AccountTest::AccountTest5()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	auto user1 = TestUser::SingleSig();
	auto user2 = TestUser::SingleSig();

	auto multi_sig = std::make_shared<MultiSignatureAuthDescriptor>(
		std::vector<std::vector<byte>> {user1->key_pair_->pub_key_, user2->key_pair_->pub_key_},
		2,
		std::vector<FlagsType> { FlagsType::eAccount, FlagsType::eTransfer });

	auto tx = blockchain_->connection_->NewTransaction(multi_sig->Signers(), this->DefaultErrorHandler);

	/*var op = AccountDevOperations.Register(multiSig);
	tx.AddOperation(op.Name, op.Args);
	tx.Sign(user1.KeyPair.PrivKey, user1.KeyPair.PubKey);
	tx.Sign(user2.KeyPair.PrivKey, user2.KeyPair.PubKey);

	bool successfully = false;
	yield return tx.PostAndWait(() = > successfully = true);
	Assert.True(successfully);*/

	return true;
}

bool AccountTest::AccountTest6()
{
	return true;
}

bool AccountTest::AccountTest7()
{
	return true;
}

bool AccountTest::AccountTest8()
{
	return true;
}

bool AccountTest::AccountTest9()
{
	return true;
}

bool AccountTest::AccountTest10()
{
	return true;
}

bool AccountTest::AccountTest11()
{
	return true;
}

bool AccountTest::AccountTest12()
{
	return true;
}

bool AccountTest::AccountTest13()
{
	return true;
}

bool AccountTest::AccountTest14()
{
	return true;
}
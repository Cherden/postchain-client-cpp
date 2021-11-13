#include "account_test.h"

#include "CoreMinimal.h" // TO-DO get rid of UE4 dependencies


void AccountTest::DefaultErrorHandler(std::string error) 
{
	UE_LOG(LogTemp, Error, TEXT("CHROMA::FT3 [%s]"), *(ChromaUtils::STDStringToFString(error)));
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

	auto tx = tx_builder->Build(signers, this->DefaultErrorHandler);
	tx->Sign(admin_user->key_pair_);
	tx->Sign(user->key_pair_);
	tx->PostAndWait(on_success);
}


bool AccountTest::AccountTest1()
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


bool AccountTest::AccountTest2()
{
	bool pass = true;

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

	pass = pass && TestOperators::NotNull(account);
	return pass;
}


bool AccountTest::AccountTest3()
{
	bool pass = true;

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

	pass = pass && TestOperators::NotNull(account);

	std::vector<FlagsType> flags = { FlagsType::eTransfer };

	auto single_sig = std::make_shared<SingleSignatureAuthDescriptor>(
		user->key_pair_->pub_key_,
		std::vector<FlagsType>{ FlagsType::eTransfer });

	account->AddAuthDescriptor(
		single_sig,
		this->EmptyCallback,
		this->DefaultErrorHandler
	);

	pass = pass && TestOperators::Equals(2, account->auth_descriptors_.size());

	return pass;
}


bool AccountTest::AccountTest4()
{
	bool pass = true;

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

	pass = pass && TestOperators::NotNull(account);

	account->AddAuthDescriptor(
		std::make_shared<SingleSignatureAuthDescriptor>(user->key_pair_->pub_key_, std::vector<FlagsType>{ FlagsType::eTransfer }),
		this->EmptyCallback, this->DefaultErrorHandler);

	pass = pass && TestOperators::Equals(1, account->auth_descriptors_.size());

	return pass;
}


// should create new multisig account
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

	auto tx = blockchain_->connection_->NewTransaction(multi_sig->signers_, this->DefaultErrorHandler);

	/*var op = AccountDevOperations.Register(multiSig);
	tx.AddOperation(op.Name, op.Args);
	tx.Sign(user1.KeyPair.PrivKey, user1.KeyPair.PubKey);
	tx.Sign(user2.KeyPair.PrivKey, user2.KeyPair.PubKey);

	bool successfully = false;
	yield return tx.PostAndWait(() = > successfully = true);
	Assert.True(successfully);*/
}

bool AccountTest::AccountTest6()
{

}

bool AccountTest::AccountTest7()
{

}

bool AccountTest::AccountTest8()
{

}

bool AccountTest::AccountTest9()
{

}

bool AccountTest::AccountTest10()
{

}

bool AccountTest::AccountTest11()
{

}

bool AccountTest::AccountTest12()
{

}

bool AccountTest::AccountTest13()
{

}

bool AccountTest::AccountTest14()
{

}
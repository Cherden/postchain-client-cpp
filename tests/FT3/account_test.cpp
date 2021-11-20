#include "account_test.h"
#include "../../FT3/User/account_dev_operations.h"
#include "../../FT3/Core/operation.h"
#include "../../FT3/Core/Blockchain/blockchain_session.h"
#include <algorithm>
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

	auto user = std::make_shared<KeyPair>(PostchainUtil::ByteVectorToHexString(private_key));

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
	auto op = AccountDevOperations::Register(multi_sig);
	tx->AddOperation(op->name_, op->args_);
	tx->Sign(user1->key_pair_->priv_key_, user1->key_pair_->pub_key_);
	tx->Sign(user2->key_pair_->priv_key_, user2->key_pair_->pub_key_);

	bool successfully = false;
	tx->PostAndWait([&successfully](std::string content) { successfully = true; });
	
	return successfully;
}

// should update account if 2 signatures provided
bool AccountTest::AccountTest6()
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

	std::shared_ptr<TransactionBuilder> tx_builder = blockchain_->NewTransactionBuilder();
	tx_builder->Add(AccountDevOperations::Register(multi_sig));
	std::shared_ptr<Transaction> tx = tx_builder->Build(multi_sig->Signers(), DefaultErrorHandler);
	tx->Sign(user1->key_pair_);
	tx->Sign(user2->key_pair_);
	bool successfully = false;
	tx->PostAndWait([&successfully](std::string content) { successfully = true; });
	if (!successfully) return false;

	std::shared_ptr<Account> account = nullptr;
	Account::GetById(multi_sig->ID(), blockchain_->NewSession(user1), [&account](std::shared_ptr<Account> _account) {
		account = _account;
	}, DefaultErrorHandler);
	if (account == nullptr) return false;

	auto auth_descriptor = std::make_shared<SingleSignatureAuthDescriptor>(
		user1->key_pair_->pub_key_,
		std::vector<FlagsType>{ FlagsType::eTransfer });

	successfully = false;
	blockchain_->NewTransactionBuilder()
		->Add(AccountOperations::AddAuthDescriptor(account->id_, account->auth_descriptors_[0]->ID(), auth_descriptor))
		->Build(account->auth_descriptors_[0]->Signers(), DefaultErrorHandler)
		->Sign(user1->key_pair_)
		->Sign(user2->key_pair_)
		->PostAndWait([&successfully](std::string content) { successfully = true; });
	if (account == nullptr) return false;

	account->Sync(EmptyCallback, DefaultErrorHandler);

	if (account->auth_descriptors_.size() == 2) return true;
	return false;
}

// should fail if only one signature provided
bool AccountTest::AccountTest7()
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
	auto op = AccountDevOperations::Register(multi_sig);
	tx->AddOperation(op->name_, op->args_);
	tx->Sign(user1->key_pair_->priv_key_, user1->key_pair_->pub_key_);
	tx->Sign(user2->key_pair_->priv_key_, user2->key_pair_->pub_key_);
	bool successfully = false;
	tx->PostAndWait([&successfully](std::string content) { successfully = true; });
	if (!successfully) return false;

	std::shared_ptr<Account> account = nullptr;
	blockchain_->NewSession(user1)->GetAccountById(multi_sig->ID(), [&account](std::shared_ptr<Account> _account) {
		account = _account;
	}, DefaultErrorHandler);
	if (account == nullptr) return false;

	successfully = false;
	account->AddAuthDescriptor(
		std::make_shared<SingleSignatureAuthDescriptor>(
			user1->key_pair_->pub_key_,
			std::vector<FlagsType>{ FlagsType::eTransfer }),
		[&successfully]() {successfully = true; },
		this->DefaultErrorHandler
	);

	if (successfully == true) return false;
	if (account->auth_descriptors_.size() != 1) return false;
	return true;
}

// Should be returned when queried by participant id
bool AccountTest::AccountTest8()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	auto user = TestUser::SingleSig();
	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	account_builder->WithParticipants(std::vector<std::shared_ptr<KeyPair>> { user->key_pair_ });

	std::shared_ptr<Account> account = nullptr;
	account_builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; }, this->DefaultErrorHandler);
	if (!account) return false;

	std::vector<std::shared_ptr<Account>> accounts;

	Account::GetByParticipantId(
		PostchainUtil::ByteVectorToHexString(user->key_pair_->pub_key_),
		blockchain_->NewSession(user),
		[&accounts](std::vector<std::shared_ptr<Account>> _accounts) {
			for (auto &acc : _accounts)
			{
				accounts.push_back(acc);
			}
		},
		DefaultErrorHandler
	);

	if (accounts.size() != 1) return false;
	std::string pub_key_str = PostchainUtil::ByteVectorToHexString(user->key_pair_->pub_key_);
	std::string signer_str = PostchainUtil::ByteVectorToHexString(accounts[0]->auth_descriptors_[0]->Signers()[0]);
	if (pub_key_str.compare(signer_str) != 0) return false;

	return true;
}

// Should return two accounts when account is participant of two accounts
bool AccountTest::AccountTest9()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	auto user_1 = TestUser::SingleSig();
	auto user_2 = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder_1 = AccountBuilder::CreateAccountBuilder(blockchain_, user_1);
	account_builder_1->WithParticipants(std::vector<std::shared_ptr<KeyPair>> { user_1->key_pair_ });
	std::shared_ptr<Account> account_1 = nullptr;
	account_builder_1->Build([&account_1](std::shared_ptr<Account> _account) { account_1 = _account; }, this->DefaultErrorHandler);

	std::shared_ptr<AccountBuilder> account_builder_2 = AccountBuilder::CreateAccountBuilder(blockchain_, user_2);
	account_builder_2->WithParticipants(std::vector<std::shared_ptr<KeyPair>> { user_2->key_pair_ });
	std::shared_ptr<Account> account_2 = nullptr;
	account_builder_2->Build([&account_2](std::shared_ptr<Account> _account) { account_2 = _account; }, this->DefaultErrorHandler);

	AddAuthDescriptorTo(account_2, user_2, user_1, EmptyCallback);

	std::vector<std::shared_ptr<Account>> accounts;
	Account::GetByParticipantId(
		PostchainUtil::ByteVectorToHexString(user_1->key_pair_->pub_key_),
		blockchain_->NewSession(user_1),
		[&accounts](std::vector<std::shared_ptr<Account>> _accounts) {
			for (auto &acc : _accounts)
			{
				accounts.push_back(acc);
			}
		},
		DefaultErrorHandler
	);

	if (accounts.size() != 2) return false;

	return true;
}

// Should return account by id
bool AccountTest::AccountTest10()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	auto user = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder_1 = AccountBuilder::CreateAccountBuilder(blockchain_, user);\
	std::shared_ptr<Account> account = nullptr;
	account_builder_1->Build([&account](std::shared_ptr<Account> _account) { account = _account; }, this->DefaultErrorHandler);

	bool success = false;
	Account::GetById(account->id_, blockchain_->NewSession(user), [&account, &success] (std::shared_ptr<Account> _account) {
		std::string account_0 = account->id_;
		std::string account_1 = _account->id_;

		std::transform(account_0.begin(), account_0.end(), account_0.begin(), ::toupper);
		std::transform(account_1.begin(), account_1.end(), account_1.begin(), ::toupper);

		if (account_0.compare(account_1) == 0)
		{
			success = true;
		}
	}, DefaultErrorHandler);

	return success;
}

// Should have only one auth descriptor after calling deleteAllAuthDescriptorsExclude
bool AccountTest::AccountTest11()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	auto user1 = TestUser::SingleSig();
	auto user2 = TestUser::SingleSig();
	auto user3 = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user1);
	account_builder->WithParticipants(std::vector<std::shared_ptr<KeyPair>> { user1->key_pair_ });
	account_builder->WithPoints(4);

	std::shared_ptr<Account> account;
	account_builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; }, this->DefaultErrorHandler);
	if (account == nullptr) return false;

	AddAuthDescriptorTo(account, user1, user2, EmptyCallback);
	AddAuthDescriptorTo(account, user1, user3, EmptyCallback);

	account->DeleteAllAuthDescriptorsExclude(user1->auth_descriptor_, EmptyCallback, DefaultErrorHandler);
	blockchain_->NewSession(user1)->GetAccountById(
		account->id_,
		[&account](std::shared_ptr<Account> _account) { account = _account; },
		this->DefaultErrorHandler
	);

	return (1 == account->auth_descriptors_.size());
}

// Should be able to register account by directly calling \'register_account\' operation
bool AccountTest::AccountTest12()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	auto user = TestUser::SingleSig();

	std::shared_ptr<ArrayValue> gtv_wrapper = AbstractValueFactory::EmptyArray();
	gtv_wrapper->Add(user->auth_descriptor_->ToGTV());
	bool success = false;
	blockchain_->Call(
		AccountOperations::Op("ft3.dev_register_account", gtv_wrapper),
		user, 
		[&success](std::string content) { success = true; },
		DefaultErrorHandler
	);

	std::shared_ptr<BlockchainSession> session = blockchain_->NewSession(user);

	std::shared_ptr<Account> account;
	session->GetAccountById(
		user->auth_descriptor_->ID(), 
		[&account](std::shared_ptr<Account> _account) { account = _account; }, 
		DefaultErrorHandler);

	if (account == nullptr) return false;
	return true;
}

// should be possible for auth descriptor to delete itself without admin flag
bool AccountTest::AccountTest13()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	auto user1 = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user1);
	account_builder->WithParticipants(std::vector<std::shared_ptr<KeyPair>> { user1->key_pair_ });
	account_builder->WithPoints(4);

	std::shared_ptr<Account> account = nullptr;
	account_builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; }, this->DefaultErrorHandler);
	if (account == nullptr) return false;

	auto key_pair = std::make_shared<KeyPair>();
	auto user2 = std::make_shared<User>(
		key_pair,
		std::make_shared<SingleSignatureAuthDescriptor>(
		key_pair->pub_key_,
		std::vector<FlagsType>{ FlagsType::eTransfer }));

	AddAuthDescriptorTo(account, user1, user2, EmptyCallback);

	std::shared_ptr<Account> account2 = nullptr;
	blockchain_->NewSession(user2)->GetAccountById(account->id_, [&account2](std::shared_ptr<Account> _account) {
		account2 = _account;
	}, DefaultErrorHandler);
	if (account2 == nullptr) return false;

	bool successfull = false;
	account2->DeleteAuthDescriptor(user2->auth_descriptor_, [&successfull]() { successfull = true; }, DefaultErrorHandler);
	if (!successfull) return false;

	account2->Sync(EmptyCallback, DefaultErrorHandler);
	if (1 == account2->auth_descriptors_.size()) return true;
	return false;
}

bool AccountTest::AccountTest14()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	auto user1 = TestUser::SingleSig();
}
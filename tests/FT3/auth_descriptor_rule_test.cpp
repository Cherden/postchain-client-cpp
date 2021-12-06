#include "auth_descriptor_rule_test.h"
#include "../../src/FT3/User/user.h"
#include "../../src/FT3/Core/transaction_builder.h"
#include "../../src/FT3/User/account.h"
#include "../../src/FT3/Core/transaction.h"
#include "../../src/FT3/User/AuthDescriptor/single_signature_auth_descriptor.h"
#include "../../src/FT3/User/AuthDescriptor/multi_signature_auth_descriptor.h"
#include "../../src/FT3/User/asset_balance.h"
#include "../../src/FT3/User/asset.h"
#include "../../src/FT3/User/account_operations.h"
#include "../TestUtil/test_util.h"

using namespace chromia;

void AuthDescriptorRuleTest::DefaultErrorHandler(std::string error)
{
	throw std::exception(error.c_str());
};


void AuthDescriptorRuleTest::SetupBlockchain()
{
	std::function<void(std::shared_ptr<Blockchain>)> on_success = [&](std::shared_ptr<Blockchain> blockchain) {
		this->blockchain_ = blockchain;
	};
	BlockchainUtil::GetDefaultBlockchain(on_success, this->DefaultErrorHandler);
	return;
}


void AuthDescriptorRuleTest::AddAuthDescriptorTo(std::shared_ptr<Account> account, std::shared_ptr<User> admin_user, std::shared_ptr<User> user, std::function<void()> on_success)
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



bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun1()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user = TestUser::SingleSig(Rules::OperationCount()->LessOrEqual(2));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });
	
	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

	successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 20, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun2()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user = TestUser::SingleSig(Rules::OperationCount()->LessThan(2));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

	successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 20, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun3()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	long long time = PostchainUtil::GetCurrentTimeMillis();
	std::shared_ptr<User> user = TestUser::SingleSig(Rules::BlockTime()->LessThan(time - 10000));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun4()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	long long time = PostchainUtil::GetCurrentTimeMillis();
	std::shared_ptr<User> user = TestUser::SingleSig(Rules::BlockTime()->LessThan(time + 10000));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });
	
	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun5()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user = TestUser::SingleSig(Rules::BlockHeight()->LessThan(10000000));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun6()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user = TestUser::SingleSig(Rules::BlockHeight()->LessThan(1));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun7()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	long long time = PostchainUtil::GetCurrentTimeMillis();
	std::shared_ptr<User> user = TestUser::SingleSig(Rules::BlockTime()->GreaterThan(time + 10000));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun8()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	long long time = PostchainUtil::GetCurrentTimeMillis();
	std::shared_ptr<User> user = TestUser::SingleSig(Rules::BlockTime()->GreaterThan(time - 10000));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun9()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user = TestUser::SingleSig(Rules::BlockHeight()->GreaterThan(1000000));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun10()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user = TestUser::SingleSig(Rules::BlockHeight()->GreaterThan(1));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun11()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user = TestUser::SingleSig(Rules::BlockHeight()->GreaterThan(1)->And()->BlockHeight()->LessThan(1000000));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun12()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user = TestUser::SingleSig(Rules::BlockHeight()->GreaterThan(1)->And()->BlockHeight()->LessThan(10));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 20, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (successful) return false;

	return true;
}



bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun13()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	long long time = PostchainUtil::GetCurrentTimeMillis();
	auto rule = Rules::BlockTime()->GreaterThan(time - 20000)
		->And()
		->BlockTime()->LessThan(time - 10000);
	std::shared_ptr<User> user = TestUser::SingleSig(rule);

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun14()
{

	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	long long time = PostchainUtil::GetCurrentTimeMillis();
	auto rule = Rules::BlockTime()->GreaterThan(time - 10000)
		->And()
		->BlockTime()->LessThan(time + 10000);
	std::shared_ptr<User> user = TestUser::SingleSig(rule);

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun15()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user1 = TestUser::SingleSig();

	std::shared_ptr<User> user2 = TestUser::SingleSig(Rules::OperationCount()->LessThan(2));

	std::shared_ptr<Account> srcAccount1 = nullptr;
	SourceAccount(blockchain_, user1, asset, [&srcAccount1](std::shared_ptr<Account> _account) { srcAccount1 = _account; });

	std::shared_ptr<Account> destAccount = nullptr;
	DestinationAccount(blockchain_, [&destAccount](std::shared_ptr<Account> _account) { destAccount = _account; });

	// add expiring auth descriptor to the account
	srcAccount1->AddAuthDescriptor(user2->auth_descriptor_, EmptyCallback, DefaultErrorHandler);

	// get the same account, but initialized with user2
	// object which contains expiring auth descriptor
	std::shared_ptr<Account> srcAccount2 = nullptr;
	blockchain_->NewSession(user2)->GetAccountById(srcAccount1->GetID(), [&srcAccount2](std::shared_ptr<Account> _account) { srcAccount2 = _account; }, DefaultErrorHandler);

	srcAccount2->Transfer(destAccount->GetID(), asset->id_, 10, EmptyCallback, DefaultErrorHandler);

	// account descriptor used by user2 object has expired.
	// this operation call will delete it.
	// any other operation, which calls require_auth internally
	// would also delete expired auth descriptor.
	srcAccount1->Transfer(destAccount->GetID(), asset->id_, 30, EmptyCallback, DefaultErrorHandler);

	srcAccount1->Sync(EmptyCallback, DefaultErrorHandler);

	if (1 != srcAccount1->auth_descriptors_.size())
	{
		return false;
	}
	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun16()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user1 = TestUser::SingleSig();

	std::shared_ptr<User> user2 = TestUser::SingleSig(Rules::OperationCount()->LessThan(10));

	std::shared_ptr<Account> srcAccount1 = nullptr;
	SourceAccount(blockchain_, user1, asset, [&srcAccount1](std::shared_ptr<Account> _account) { srcAccount1 = _account; });

	std::shared_ptr<Account> destAccount = nullptr;
	DestinationAccount(blockchain_, [&destAccount](std::shared_ptr<Account> _account) { destAccount = _account; });

	// add expiring auth descriptor to the account
	AddAuthDescriptorTo(srcAccount1, user1, user2, EmptyCallback);

	// get the same account, but initialized with user2
	// object which contains expiring auth descriptor
	std::shared_ptr<Account> srcAccount2 = nullptr;
	blockchain_->NewSession(user2)->GetAccountById(srcAccount1->GetID(), [&srcAccount2](std::shared_ptr<Account> _account) { srcAccount2 = _account; }, DefaultErrorHandler);

	// perform transfer with expiring auth descriptor.
	// auth descriptor didn't expire, because it's only used 1 out of 10 times.
	srcAccount2->Transfer(destAccount->GetID(), asset->id_, 10, EmptyCallback, DefaultErrorHandler);

	// perform transfer using auth descriptor without rules
	srcAccount1->Transfer(destAccount->GetID(), asset->id_, 30, EmptyCallback, DefaultErrorHandler);

	srcAccount1->Sync(EmptyCallback, DefaultErrorHandler);

	if (2 != srcAccount1->auth_descriptors_.size())
	{
		return false;
	}
	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun17()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user1 = TestUser::SingleSig();

	std::shared_ptr<User> user2 = TestUser::SingleSig(Rules::OperationCount()->LessOrEqual(1));

	std::shared_ptr<User> user3 = TestUser::SingleSig(Rules::OperationCount()->LessOrEqual(1));

	std::shared_ptr<Account> srcAccount1 = nullptr;
	SourceAccount(blockchain_, user1, asset, [&srcAccount1](std::shared_ptr<Account> _account) { srcAccount1 = _account; });

	std::shared_ptr<Account> destAccount = nullptr;
	DestinationAccount(blockchain_, [&destAccount](std::shared_ptr<Account> _account) { destAccount = _account; });

	AddAuthDescriptorTo(srcAccount1, user1, user2, EmptyCallback);
	AddAuthDescriptorTo(srcAccount1, user1, user3, EmptyCallback);

	std::shared_ptr<Account> srcAccount2 = nullptr;
	blockchain_->NewSession(user2)->GetAccountById(srcAccount1->GetID(), [&srcAccount2](std::shared_ptr<Account> _account) { srcAccount2 = _account; }, DefaultErrorHandler);

	srcAccount2->Transfer(destAccount->GetID(), asset->id_, 50, EmptyCallback, DefaultErrorHandler);

	// this call will trigger deletion of expired auth descriptor (attached to user2)
	srcAccount1->Transfer(destAccount->GetID(), asset->id_, 100, EmptyCallback, DefaultErrorHandler);

	srcAccount1->Sync(EmptyCallback, DefaultErrorHandler);

	if (2 != srcAccount1->auth_descriptors_.size()) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun18()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user1 = TestUser::SingleSig();

	std::shared_ptr<User> user2 = TestUser::SingleSig(Rules::OperationCount()->LessOrEqual(1));

	std::shared_ptr<User> user3 = TestUser::SingleSig(Rules::OperationCount()->LessOrEqual(1));

	std::shared_ptr<Account> account = nullptr;
	SourceAccount(blockchain_, user1, asset, [&account](std::shared_ptr<Account> _account) { account = _account; });

	AddAuthDescriptorTo(account, user1, user2, EmptyCallback);
	AddAuthDescriptorTo(account, user1, user3, EmptyCallback);

	account->Sync(EmptyCallback, DefaultErrorHandler);

	if (3 != account->auth_descriptors_.size()) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun19()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user1 = TestUser::SingleSig();

	std::shared_ptr<User> user2 = TestUser::SingleSig(Rules::OperationCount()->LessOrEqual(1));

	std::shared_ptr<User> user3 = TestUser::SingleSig(Rules::OperationCount()->LessOrEqual(1));

	std::shared_ptr<Account> account = nullptr;
	SourceAccount(blockchain_, user1, asset, [&account](std::shared_ptr<Account> _account) { account = _account; });

	account->AddAuthDescriptor(user2->auth_descriptor_, EmptyCallback, DefaultErrorHandler);
	account->AddAuthDescriptor(user3->auth_descriptor_, EmptyCallback, DefaultErrorHandler);

	account->DeleteAllAuthDescriptorsExclude(user1->auth_descriptor_, EmptyCallback, DefaultErrorHandler);
	if (1 != account->auth_descriptors_.size()) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun20()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user1 = TestUser::SingleSig();
	std::shared_ptr<User> user2 = TestUser::SingleSig();

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user1, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	SourceAccount(blockchain_, user2, asset, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successfull = false;
	account1->DeleteAuthDescriptor(user2->auth_descriptor_, [&successfull]() { successfull = true; }, DefaultErrorHandler);
	if (successfull) return false;

	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun21()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	std::shared_ptr<User> user1 = TestUser::SingleSig();
	std::shared_ptr<User> user2 = TestUser::SingleSig();

	std::shared_ptr<Account> account = nullptr;
	SourceAccount(blockchain_, user1, asset, [&account](std::shared_ptr<Account> _account) { account = _account; });
	account->AddAuthDescriptor(user2->auth_descriptor_, EmptyCallback, DefaultErrorHandler);
	account->DeleteAuthDescriptor(user2->auth_descriptor_, EmptyCallback, DefaultErrorHandler);

	if (1 != account->auth_descriptors_.size()) return false;
	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun22()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	auto rule = Rules::BlockHeight()->GreaterThan(1)
		->And()
		->BlockHeight()->GreaterThan(10000)
		->And()
		->BlockTime()->GreaterOrEqual(122222999);

	std::shared_ptr<User> user = TestUser::SingleSig(rule);

	std::shared_ptr<Account> account = nullptr;
	SourceAccount(blockchain_, user, asset, [&account](std::shared_ptr<Account> _account) { account = _account; });

	if (1 != account->auth_descriptors_.size()) return false;
	return true;
}


bool AuthDescriptorRuleTest::AuthDescriptorRuleTestRun23()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	CreateAsset(blockchain_, [&asset](std::shared_ptr<Asset> _asset) { asset = _asset; });
	if (asset == nullptr) return false;

	auto rule = Rules::BlockHeight()->GreaterThan(1)
		->And()
		->BlockHeight()->GreaterThan(10000)
		->And()
		->BlockTime()->GreaterOrEqual(122222999);

	for (int i = 0; i < 400; i++)
	{
		rule = rule->And()->BlockHeight()->GreaterOrEqual(i);
	}
	std::shared_ptr<User> user = TestUser::SingleSig(rule);

	std::shared_ptr<Account> account = nullptr;
	SourceAccount(blockchain_, user, asset, [&account](std::shared_ptr<Account> _account) { account = _account; });

	if (account != nullptr) return false;

	return true;
}
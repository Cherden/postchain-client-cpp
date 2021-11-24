#include "auth_descriptor_rule_test.h"
#include "../../FT3/User/user.h"
#include "../../FT3/Core/transaction_builder.h"
#include "../../FT3/User/account.h"
#include "../../FT3/Core/transaction.h"
#include "../../FT3/User/AuthDescriptor/single_signature_auth_descriptor.h"
#include "../../FT3/User/AuthDescriptor/multi_signature_auth_descriptor.h"
#include "../../FT3/User/asset_balance.h"

//#include "../../FT3/User/account_dev_operations.h"
//#include "../../FT3/Core/operation.h"
//#include "../../FT3/Core/Blockchain/blockchain_session.h"
//#include <algorithm>

#include "../TestUtil/test_util.h"
#include "../../FT3/User/asset.h"
//#include "../../FT3/User/asset_balance.h"

#include "CoreMinimal.h" // TO-DO get rid of UE4 dependencies

void AuthDescriptorRuleTest::DefaultErrorHandler(std::string error)
{
	UE_LOG(LogTemp, Error, TEXT("CHROMA::Error [%s]"), *(ChromaUtils::STDStringToFString(error)));
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

	UE_LOG(LogTemp, Warning, TEXT("CHROMA::CreateAsset id:  [%s]  issuing chain id: [%s]"), *(ChromaUtils::STDStringToFString(asset->id_)), *(ChromaUtils::STDStringToFString(asset->issuing_chain_rid_)));

	//auto op_count = Rules::OperationCount();
	//auto rule = op_count.LessOrEqual(2);
	//std::shared_ptr<RuleExpression> rule_ptr(&rule);
	//std::shared_ptr<IAuthdescriptorRule> irule_ptr = std::shared_ptr<RuleExpression>(rule_ptr);
	
	std::shared_ptr<User> user = TestUser::SingleSig();

	//auto gtv = user->auth_descriptor_->ToGTV();

	//std::shared_ptr<Account> account = nullptr;
	//blockchain_->RegisterAccount(user->auth_descriptor_, user, [&account](std::shared_ptr<Account> _account) {
	//	account = _account;
	//}, this->DefaultErrorHandler);


	//if (account == nullptr)
	//{
	//	return false;
	//}

	//bool success = false;
	//AssetBalance::GiveBalance(account->id_, asset->id_, 200, this->blockchain_, [&success]() {success = true; }, DefaultErrorHandler);
	// -----------------------------------------------------------------------------

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

	auto op_count = Rules::OperationCount();
	auto rule = op_count.LessThan(2);
	std::shared_ptr<User> user = TestUser::SingleSig(std::dynamic_pointer_cast<IAuthdescriptorRule>(std::shared_ptr<RuleExpression>(&rule)));

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

	auto op_count = Rules::OperationCount();
	long long time = PostchainUtil::GetCurrentTimeMillils();
	auto rule = op_count.LessThan(time - 10000);
	std::shared_ptr<User> user = TestUser::SingleSig(std::dynamic_pointer_cast<IAuthdescriptorRule>(std::shared_ptr<RuleExpression>(&rule)));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });

	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

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

	auto op_count = Rules::OperationCount();
	long long time = PostchainUtil::GetCurrentTimeMillils();
	auto rule = op_count.LessThan(time + 10000);
	std::shared_ptr<User> user = TestUser::SingleSig(std::dynamic_pointer_cast<IAuthdescriptorRule>(std::shared_ptr<RuleExpression>(&rule)));

	std::shared_ptr<Account> account1 = nullptr;
	SourceAccount(blockchain_, user, asset, [&account1](std::shared_ptr<Account> _account) { account1 = _account; });
	
	std::shared_ptr<Account> account2 = nullptr;
	DestinationAccount(blockchain_, [&account2](std::shared_ptr<Account> _account) { account2 = _account; });

	bool successful = false;
	account1->Transfer(account2->GetID(), asset->id_, 10, [&successful]() { successful = true; }, DefaultErrorHandler);
	if (!successful) return false;

	return true;
}
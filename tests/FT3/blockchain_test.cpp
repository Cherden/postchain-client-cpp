#include "blockchain_test.h"
#include "../../src/FT3/User/AuthDescriptor/auth_descriptor_rule.h"
#include "../../src/FT3/User/asset.h"
#include "../../src/FT3/User/user.h"
#include "../../src/FT3/User/account.h"
#include "../../src/FT3/Core/Blockchain/blockchain_session.h"
#include "../../src/FT3/Core/Blockchain/blockchain_info.h"
#include "../../src/FT3/Core/key_pair.h"
#include "../../src/PostchainClient/postchain_util.h"
#include "../TestUtil/blockchain_util.h"
#include "../TestUtil/test_user.h"
#include "../TestUtil/test_util.h"
#include "../TestUtil/account_builder.h"


void BlockchainTest::DefaultErrorHandler(std::string error)
{
	throw std::exception(error.c_str());
};


void BlockchainTest::SetupBlockchain()
{
	std::function<void(std::shared_ptr<Blockchain>)> on_success = [&](std::shared_ptr<Blockchain> blockchain) {
		this->blockchain_ = blockchain;
	};
	BlockchainUtil::GetDefaultBlockchain(on_success, this->DefaultErrorHandler);
	return;
}


bool BlockchainTest::BlockchainTestRun1()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::string info_name = "";
	BlockchainInfo::GetInfo(blockchain_->connection_,
		[&info_name](std::shared_ptr<BlockchainInfo> info) { info_name = info->name_; }, DefaultErrorHandler);

	if (info_name.compare("Unity FT3") != 0) return	false;

	return true;
}


bool BlockchainTest::BlockchainTestRun2()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<User> user = TestUser::SingleSig();
	std::shared_ptr<BlockchainSession> session = blockchain_->NewSession(user);

	std::shared_ptr<Account> account = nullptr;
	std::shared_ptr<Account> foundAccount = nullptr;

	blockchain_->RegisterAccount(user->auth_descriptor_, user, [&account](std::shared_ptr<Account> _account) { account = _account; }, DefaultErrorHandler);
	session->GetAccountById(account->id_, [&foundAccount](std::shared_ptr<Account> _account) { foundAccount = _account; }, DefaultErrorHandler);

	if (PostchainUtil::ToUpper(account->id_).compare(PostchainUtil::ToUpper(account->id_)) != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool BlockchainTest::BlockchainTestRun3()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<User> user = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	account_builder->WithParticipants(std::vector<std::shared_ptr<KeyPair>> {user->key_pair_});

	std::shared_ptr<Account> account = nullptr;
	std::vector<std::shared_ptr<Account>> found_accounts;

	account_builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; });

	blockchain_->GetAccountsByParticipantId(
		PostchainUtil::ByteVectorToHexString(user->key_pair_->pub_key_),
		user,
		[&found_accounts](std::vector<std::shared_ptr<Account>> _found_accounts) { found_accounts = _found_accounts; },
		DefaultErrorHandler
	);

	if (1 != found_accounts.size()) return false;
	if (PostchainUtil::ToUpper(account->id_).compare(PostchainUtil::ToUpper(found_accounts[0]->id_))) return false;

	return true;
}


bool BlockchainTest::BlockchainTestRun4()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<User> user = TestUser::SingleSig();

	std::shared_ptr<AccountBuilder> account_builder = AccountBuilder::CreateAccountBuilder(blockchain_, user);
	account_builder->WithParticipants(std::vector<std::shared_ptr<KeyPair>> {user->key_pair_});

	std::shared_ptr<Account> account = nullptr;
	std::vector<std::shared_ptr<Account>> found_accounts;

	account_builder->Build([&account](std::shared_ptr<Account> _account) { account = _account; });

	blockchain_->GetAccountsByAuthDescriptorId(
		user->auth_descriptor_->ID(),
		user,
		[&found_accounts](std::vector<std::shared_ptr<Account>> _found_accounts) { found_accounts = _found_accounts; },
		DefaultErrorHandler
	);

	if (1 != found_accounts.size()) return false;
	if (PostchainUtil::ToUpper(account->id_).compare(PostchainUtil::ToUpper(found_accounts[0]->id_))) return false;

	return true;
}


bool BlockchainTest::BlockchainTestRun5()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::string chain_id = TestUtil::GenerateId();

	bool successfull = false;
	blockchain_->LinkChain(chain_id, [&successfull](std::string content) { successfull = true; }, DefaultErrorHandler);
	if (!successfull) return false;

	bool is_linked = false;
	blockchain_->IsLinkedWithChain(chain_id, [&is_linked](bool _is_linked) { is_linked = _is_linked; }, DefaultErrorHandler);

	if (!is_linked) return false;

	return true;
}


bool BlockchainTest::BlockchainTestRun6()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::string chain_id_1 = TestUtil::GenerateId();
	std::string chain_id_2 = TestUtil::GenerateId();

	bool successfull = false;
	blockchain_->LinkChain(chain_id_1, [&successfull](std::string content) { successfull = true; }, DefaultErrorHandler);
	if (!successfull) return false;

	successfull = false;
	blockchain_->LinkChain(chain_id_2, [&successfull](std::string content) { successfull = true; }, DefaultErrorHandler);
	if (!successfull) return false;

	std::vector<std::string> linked_chains;
	blockchain_->GetLinkedChainsIds([&linked_chains](std::vector<std::string> _linked_chains) {linked_chains = _linked_chains; }, DefaultErrorHandler);

	bool chain_1_contained = false;
	bool chain_2_contained = false;

	for (auto & item : linked_chains)
	{
		if (PostchainUtil::ToUpper(item).compare(PostchainUtil::ToUpper(chain_id_1))) chain_1_contained = true;
		if (PostchainUtil::ToUpper(item).compare(PostchainUtil::ToUpper(chain_id_2))) chain_2_contained = true;
	}

	if (!chain_1_contained) return false;
	if (!chain_2_contained) return false;

	return true;
}


bool BlockchainTest::BlockchainTestRun7()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	bool is_linked = false;
	blockchain_->IsLinkedWithChain(
		TestUtil::GenerateId(),
		[&is_linked](bool _is_linked) {is_linked = _is_linked; },
		DefaultErrorHandler
	);

	if (is_linked) return false;

	return true;
}


bool BlockchainTest::BlockchainTestRun8()
{
	SetupBlockchain();
	if (this->blockchain_ == nullptr)
	{
		return false;
	}

	std::shared_ptr<Asset> asset = nullptr;
	Asset::Register(
		TestUtil::GenerateAssetName(),
		TestUtil::GenerateId(),
		blockchain_,
		[&asset](std::shared_ptr<Asset> _asset) {asset = _asset; },
		DefaultErrorHandler
	);

	std::shared_ptr<Asset> found_asset = nullptr;
	blockchain_->GetAssetById(asset->id_, [&found_asset](std::shared_ptr<Asset> _found_asset) {found_asset = _found_asset; }, DefaultErrorHandler);

	if (PostchainUtil::ToUpper(asset->id_).compare(PostchainUtil::ToUpper(found_asset->id_)) != 0) return false;

	return true;
}
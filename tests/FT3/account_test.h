#pragma once 

//#ifdef FT3_TESTS

//#include <gtest/gtest.h>

//#include "GoogleTest/include/gtest/gtest.h"


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
#include "../../FT3/User/AuthDescriptor/single_signature_auth_descriptor.h"
#include "../../FT3/User/AuthDescriptor/multi_signature_auth_descriptor.h"
#include "../../../ChromaUnreal/Utils.h"


using namespace chromia::postchain::ft3;
using namespace chromia::postchain;

class AccountTest
{
private:
	std::shared_ptr<Blockchain> blockchain_ = nullptr;

	//std::function<void(std::string error)> default_error_handler_ptr = std::bind(&);

	void SetupBlockchain();

	static void EmptyCallback() {}

	static void DefaultErrorHandler(std::string error);

	void AddAuthDescriptorTo(std::shared_ptr<Account> account, std::shared_ptr<User> admin_user, std::shared_ptr<User> user, std::function<void()> on_success);

public:

	// Correctly creates keypair
	bool AccountTest1();

	// Register account on blockchain
	bool AccountTest2();

	// Can add new auth descriptor if has account edit rights
	bool AccountTest3();

	// Cannot add new auth descriptor if account doesn't have account edit rights
	bool AccountTest4();

	// should create new multisig account
	bool AccountTest5();

	// should update account if 2 signatures provided
	bool AccountTest6();

	// should fail if only one signature provided
	bool AccountTest7();

	// should be returned when queried by participant id
	bool AccountTest8();

	// should return two accounts when account is participant of two accounts
	bool AccountTest9();

	// should return account by id
	bool AccountTest10();

	// should have only one auth descriptor after calling deleteAllAuthDescriptorsExclude
	bool AccountTest11();

	// should be able to register account by directly calling \'register_account\' operation
	bool AccountTest12();

	// should be possible for auth descriptor to delete itself without admin flag
	bool AccountTest13();

	// shouldn't be possible for auth descriptor to delete other auth descriptor without admin flag
	bool AccountTest14();
};

//#endif // FT3_TESTS
#pragma once 

//#ifdef FT3_TESTS

#include <functional>
#include <string>
#include "../../src/common.h"
#include "../../FT3/forward_declarations.h"
#include "../../FT3/User/AuthDescriptor/auth_descriptor_rule.h"
#include "../../FT3/User/asset.h"
#include "../../FT3/User/user.h"
#include "../../FT3/User/account.h"
#include "../../FT3/Core/Blockchain/blockchain_session.h"
#include "../../FT3/Core/Blockchain/blockchain_info.h"
#include "../../FT3/Core/key_pair.h"
#include "../../src/postchain_util.h"
#include "../TestUtil/blockchain_util.h"
#include "../TestUtil/test_user.h"
#include "../TestUtil/test_util.h"
#include "../TestUtil/account_builder.h"
#include "../TestUtil/test_operators.h"

using namespace chromia::postchain::ft3;
using namespace chromia::postchain;

class BlockchainTest
{
private:
	std::shared_ptr<Blockchain> blockchain_ = nullptr;

	void SetupBlockchain();

	static void EmptyCallback() {}

	static void DefaultErrorHandler(std::string error);

public:

	// should provide info
	bool BlockchainTestRun1();

	// should be able to register an account
	bool BlockchainTestRun2();

	// should return account by participant id
	bool BlockchainTestRun3();

	// should return account by auth descriptor id
	bool BlockchainTestRun4();

	// should be able to link other chain
	bool BlockchainTestRun5();

	// should be able to link multiple chains
	bool BlockchainTestRun6();

	// should return false when isLinkedWithChain is called for unknown chain id
	bool BlockchainTestRun7();

	// should return asset queried by id
	bool BlockchainTestRun8();
};

//#endif // FT3_TESTS
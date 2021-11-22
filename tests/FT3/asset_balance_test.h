#pragma once 

//#ifdef FT3_TESTS

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

class AssetBalanceTest
{
private:
	std::shared_ptr<Blockchain> blockchain_ = nullptr;

	void SetupBlockchain();

	static void EmptyCallback() {}

	static void DefaultErrorHandler(std::string error);

public:

	bool AssetBalanceTestRun();
};

//#endif // FT3_TESTS
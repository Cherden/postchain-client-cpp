#pragma once 

//#ifdef FT3_TESTS

#include <string>
#include "../../src/PostchainClient/common.h"
#include "../TestUtil/blockchain_util.h"
#include "../TestUtil/test_user.h"
#include "../TestUtil/account_builder.h"
#include "../../src/FT3/forward_declarations.h"


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
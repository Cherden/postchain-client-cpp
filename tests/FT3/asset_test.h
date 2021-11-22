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
#include "../../FT3/forward_declarations.h"

using namespace chromia::postchain::ft3;
using namespace chromia::postchain;

class AssetTest
{
private:
	std::shared_ptr<Blockchain> blockchain_ = nullptr;

	void SetupBlockchain();

	static void EmptyCallback() {}

	static void DefaultErrorHandler(std::string error);

public:

	// should be successfully registered
	bool AssetTestRun1();

	// should be returned when queried by name
	bool AssetTestRun2();

	// should be returned when queried by id
	bool AssetTestRun3();

	// should return all the assets registered
	bool AssetTestRun4();
};

//#endif // FT3_TESTS
#pragma once 

#include <functional>
#include <string>
#include "../../src/common.h"
#include "../../FT3/forward_declarations.h"


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

#pragma once 

#include <functional>
#include <string>
#include "../../src/PostchainClient/common.h"
#include "../../src/FT3/forward_declarations.h"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::ft3;

class TransferTest
{
private:
	std::shared_ptr<Blockchain> blockchain_ = nullptr;

	void SetupBlockchain();

	static void EmptyCallback() {}

	static void DefaultErrorHandler(std::string error);

public:

	// should succeed when balance is higher than amount to transfer
	bool TransferTestRun1();

	// should fail when balance is lower than amount to transfer
	bool TransferTestRun2();

	// should fail if auth descriptor doesn't have transfer rights
	bool TransferTestRun3();

	// should succeed if transferring tokens to a multisig account
	bool TransferTestRun4();

	// should succeed burning tokens
	bool TransferTestRun5();
};

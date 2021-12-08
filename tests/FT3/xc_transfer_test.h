#pragma once 

#include <functional>
#include <string>
#include "../../src/PostchainClient/common.h"
#include "../../src/FT3/forward_declarations.h"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::ft3;

class XCTransferTest
{
private:
	std::shared_ptr<Blockchain> blockchain_ = nullptr;

	void SetupBlockchain();

	static void EmptyCallback() {}

	static void DefaultErrorHandler(std::string error);

public:

	// Cross-chain transfer
	bool XCTransferTestRun1();
};

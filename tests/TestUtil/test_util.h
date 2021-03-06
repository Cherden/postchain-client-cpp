#pragma once 

#include <string>
#include <vector>
#include "../../src/FT3/forward_declarations.h"

using namespace chromia::postchain::ft3;

class TestUtil
{
public:
	static int GenerateNumber(int max = 10000);

	static std::string GenerateAssetName(std::string prefix = "CHROMA");

	static std::string GenerateId();

	static std::string BlockchainAccountId(std::string chainId);
};
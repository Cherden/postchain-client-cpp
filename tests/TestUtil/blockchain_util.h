#pragma once 

//#ifdef FT3_TESTS

#include "directory_service_util.h"
#include "../../FT3/Core/Blockchain/blockchain.h"

using namespace chromia::postchain::ft3;

class BlockchainUtil
{
public:
	static void GetDefaultBlockchain(std::function<void(std::shared_ptr<Blockchain>)> on_success, std::function<void(std::string)> on_error);
};


//#endif // FT3_TESTS
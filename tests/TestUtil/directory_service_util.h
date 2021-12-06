#pragma once 

//#ifdef FT3_TESTS

#include "fake_directory_service_util.h"
#include "../../src/FT3/Core/Blockchain/blockchain.h"
#include "../../src/FT3/Core/chain_connection_info.h"

using namespace chromia::postchain::ft3;

class DirectoryServiceUtil
{
public:
	static std::shared_ptr<DirectoryService> GetDefaultDirectoryService(std::string chain_id, std::string node_url);
};

//#endif // FT3_TESTS
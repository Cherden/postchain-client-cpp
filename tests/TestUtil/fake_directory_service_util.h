#pragma once 

//#ifdef FT3_TESTS

#include "../../FT3/Core/Blockchain/blockchain.h"
#include "../../FT3/Core/chain_connection_info.h"
#include "../../FT3/Core/Blockchain/directory_service.h"

using namespace chromia::postchain::ft3;

class FakeDirectoryService : public DirectoryService
{
private:

	std::vector<std::shared_ptr<ChainConnectionInfo>> chain_infos_;

public: 

	FakeDirectoryService(std::vector<std::shared_ptr<ChainConnectionInfo>> chain_infos);

	std::shared_ptr<ChainConnectionInfo> GetChainConnectionInfo(std::string id);
};

//#endif // FT3_TESTS
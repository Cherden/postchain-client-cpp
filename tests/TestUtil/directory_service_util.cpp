
//#ifdef FT3_TESTS

#include "directory_service_util.h"

std::shared_ptr<DirectoryService> DirectoryServiceUtil::GetDefaultDirectoryService(std::string chain_id, std::string node_url)
{
	auto connection = std::make_shared<ChainConnectionInfo>(chain_id, node_url);

	std::vector<std::shared_ptr<ChainConnectionInfo>> args = { connection };
	return std::make_shared<FakeDirectoryService>(args);
}

//#endif // FT3_TESTS
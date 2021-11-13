//#ifdef FT3_TESTS

#include "fake_directory_service_util.h"

FakeDirectoryService::FakeDirectoryService(std::vector<std::shared_ptr<ChainConnectionInfo>> chain_infos)
{
	this->chain_infos_.clear();
	this->chain_infos_.insert(this->chain_infos_.begin(), chain_infos.begin(), chain_infos.end());
}

std::shared_ptr<ChainConnectionInfo> FakeDirectoryService::GetChainConnectionInfo(std::string id)
{
	for (auto & info : this->chain_infos_)
	{
		if (info->chain_id.compare(id) == 0)
		{
			return info;
		}
	}
	return nullptr;
}

//#endif // FT3_TESTS
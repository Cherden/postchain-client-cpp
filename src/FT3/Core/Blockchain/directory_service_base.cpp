#include "directory_service_base.h"

namespace chromia {
namespace postchain {
namespace ft3 {

DirectoryServiceBase::DirectoryServiceBase(std::vector<std::shared_ptr<ChainConnectionInfo>> chain_infos)
{
	this->chain_infos_ = chain_infos;
}

std::shared_ptr<ChainConnectionInfo> DirectoryServiceBase::GetChainConnectionInfo(std::string id)
{
	for (std::shared_ptr<ChainConnectionInfo> info : this->chain_infos_)
	{
		if (info->chain_id.compare(id) == 0) {
			return info;
		}
	}

	return nullptr;
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
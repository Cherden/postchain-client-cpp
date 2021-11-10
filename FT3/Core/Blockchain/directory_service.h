#pragma once

#include "../chain_connection_info.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class DirectoryService
{
public:
	virtual std::shared_ptr<ChainConnectionInfo> GetChainConnectionInfo(std::string id) { return nullptr; };
	virtual ~DirectoryService() = default;
};

} // namespace ft3
} // namespace postchain
} // namespace chromia
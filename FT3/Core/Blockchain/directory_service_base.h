#include "directory_service.h"
#include <vector>
#include <string>

namespace chromia {
namespace postchain {
namespace ft3 {
class DirectoryServiceBase : DirectoryService
{
public:
	DirectoryServiceBase(std::vector<std::shared_ptr<ChainConnectionInfo>> chain_infos);

	std::shared_ptr<ChainConnectionInfo> GetChainConnectionInfo(std::string id);

private:
	std::vector<std::shared_ptr<ChainConnectionInfo>> chain_infos_;
};

} // namespace ft3
} // namespace postchain
} // namespace chromia
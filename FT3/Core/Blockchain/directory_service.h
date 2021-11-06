#include "../chain_connection_info.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class DirectoryService
{
public:
	virtual std::shared_ptr<ChainConnectionInfo> GetChainConnectionInfo(std::string id) {};
};

} // namespace ft3
} // namespace postchain
} // namespace chromia
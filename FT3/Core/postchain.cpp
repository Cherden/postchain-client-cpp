#include "postchain.h"
#include "../Core/chain_connection_info.h"

namespace chromia {
namespace postchain {
namespace ft3 {

Postchain::Postchain(std::string url)
{
	this->url_ = url;
}

void Postchain::Blockchain(std::string id, std::function<void(std::shared_ptr<ft3::Blockchain>)> on_success, std::function<void(std::string)> on_error)
{
	std::shared_ptr<DirectoryServiceBase> directory_service = std::make_shared<DirectoryServiceBase>(
		std::vector<std::shared_ptr<ChainConnectionInfo>> { std::make_shared<ChainConnectionInfo>(id, url_) }
	);

	return Blockchain::Initialize(id, directory_service, on_success, on_error);
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
#include "blockchain_util.h"


void BlockchainUtil::GetDefaultBlockchain(std::function<void(std::shared_ptr<Blockchain>)> on_success, std::function<void(std::string)> on_error)
{
	Blockchain::Initialize(
		"849AD8C9AC720A21962187D0BDA6168DA274E1D17D39AAD513559171FDDC6914",
		DirectoryServiceUtil::GetDefaultDirectoryService("849AD8C9AC720A21962187D0BDA6168DA274E1D17D39AAD513559171FDDC6914", "http://localhost:7740"),
		on_success,
		on_error
	);
}

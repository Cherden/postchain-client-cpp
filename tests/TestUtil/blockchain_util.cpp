//#ifdef FT3_TESTS

#include "blockchain_util.h"

void BlockchainUtil::GetDefaultBlockchain(std::function<void(std::shared_ptr<Blockchain>)> on_success, std::function<void(std::string)> on_error)
{
	Blockchain::Initialize(
		"5759EB34C39B4D34744EC324DFEFAC61526DCEB37FB05D22EB7C95A184380205",
		DirectoryServiceUtil::GetDefaultDirectoryService("5759EB34C39B4D34744EC324DFEFAC61526DCEB37FB05D22EB7C95A184380205", "http://localhost:7740"),
		on_success,
		on_error
	);
}

//#endif // FT3_TESTS
#include "blockchain_info.h"
#include "CoreMinimal.h"
#include "../../../../nlohmann/json.hpp"
#include "../../../../ChromaUnreal/Utils.h"

namespace chromia {
namespace postchain {
namespace ft3 {

BlockchainInfo::BlockchainInfo(std::string name, std::string website, std::string description, RateLimitInfo rate_limit_info)
{
	this->name_ = name;
	this->website_ = website;
	this->description_ = description;
	this->rate_limit_info_ = rate_limit_info;
}

BlockchainInfo::BlockchainInfo(std::string name, std::string website, std::string description,
	int rate_limit_active, int rate_limit_max_points, int rate_limit_recovery_time, int rate_limit_points_at_account_creation)
{
	this->name_ = name;
	this->website_ = website;
	this->description_ = description;
	this->rate_limit_info_ = RateLimitInfo(
		rate_limit_active == 1,
		rate_limit_max_points,
		rate_limit_recovery_time,
		rate_limit_points_at_account_creation
	);
}

void BlockchainInfo::GetInfo(std::shared_ptr<BlockchainClient> connection, std::function<void(std::shared_ptr<BlockchainInfo>)> on_success, std::function<void(std::string)> on_error)
{
	std::function<void(std::string)> on_success_wrapper = [on_success](std::string content) {
		UE_LOG(LogTemp, Error, TEXT("CHROMA::BlockchainInfo::GetInfo callback wrapper: %s"),  *(ChromaUtils::STDStringToFString(content)));
		
		nlohmann::json json_content = nlohmann::json::parse(content);
		
		std::shared_ptr<BlockchainInfo> info = std::make_shared<BlockchainInfo>();
		on_success(info);
	};

	connection->Query("ft3.get_blockchain_info", {}, on_success_wrapper, on_error);
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
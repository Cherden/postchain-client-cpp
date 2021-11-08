#include "blockchain_info.h"

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

void BlockchainInfo::GetInfo(std::shared_ptr<BlockchainClient> connection, std::function<void(BlockchainInfo)> on_success, std::function<void(std::string)> on_error)
{
	//connection->Query()
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
#include "blockchain_info.h"
#include "../../../PostchainClient/postchain_util.h"
#include "../../../nlohmann/json.hpp"

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
	std::function<void(std::string)> on_success_wrapper = [&on_success](std::string content) {
		nlohmann::json json_content = nlohmann::json::parse(content);

		std::string name = PostchainUtil::GetSafeJSONString(json_content, std::string("name"));
		std::string website = PostchainUtil::GetSafeJSONString(json_content, std::string("website"));
		std::string description = PostchainUtil::GetSafeJSONString(json_content, std::string("description"));
		int rate_limit_active = PostchainUtil::GetSafeJSONInt(json_content, std::string("rate_limit_active"));
		int rate_limit_max_points = PostchainUtil::GetSafeJSONInt(json_content, std::string("rate_limit_max_points"));
		int rate_limit_recovery_time = PostchainUtil::GetSafeJSONInt(json_content, std::string("rate_limit_recovery_time"));
		int rate_limit_points_at_account_creation = PostchainUtil::GetSafeJSONInt(json_content, std::string("rate_limit_points_at_account_creation"));

		std::shared_ptr<BlockchainInfo> info = std::make_shared<BlockchainInfo>(
			name, website, description, rate_limit_active, rate_limit_max_points, rate_limit_recovery_time, rate_limit_points_at_account_creation);
		on_success(info);
	};

	connection->Query("ft3.get_blockchain_info", {}, on_success_wrapper, on_error);
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
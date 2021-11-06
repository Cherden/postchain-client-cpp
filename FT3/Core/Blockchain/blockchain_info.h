#include <string>
#include "rate_limit_info.h"

namespace chromia {
namespace postchain {
namespace ft3 {
class BlockchainInfo
{
public:
	std::string name_;
	std::string website_;
	std::string description_;
	RateLimitInfo rate_limit_info_;

	BlockchainInfo() {};

	BlockchainInfo(std::string name, std::string website, std::string description, RateLimitInfo rate_limit_info);

	BlockchainInfo(std::string name, std::string website, std::string description,
		int rate_limit_active, int rate_limit_max_points, int rate_limit_recovery_time, int rate_limit_points_at_account_creation);

	//static void GetInfo(BlockchainClient connection);
};
} // namespace ft3
} //namespace postchain
} //namespace chromia
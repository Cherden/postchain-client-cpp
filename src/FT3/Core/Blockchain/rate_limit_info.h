#pragma once

namespace chromia {
namespace postchain {
namespace ft3 {
class RateLimitInfo
{
public:
	bool is_active_;
	int max_points_;
	int recovery_time_;
	int points_at_account_creation_;

	RateLimitInfo() {};

	RateLimitInfo(bool is_active, int max_points, int recovery_time, int points_at_account_creation)
	{
		this->is_active_ = is_active;
		this->max_points_ = max_points;
		this->recovery_time_ = recovery_time;
		this->points_at_account_creation_ = points_at_account_creation;
	}
};
} // namespace ft3
} //namespace postchain
} //namespace chromia
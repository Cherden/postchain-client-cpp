#pragma once

#include <memory>
#include <string>
#include <functional>
#include "../forward_declarations.h"

namespace chromia {
namespace postchain {
namespace ft3 {
class RateLimit
{
public:
	int points_;
    long last_update_;

	RateLimit(int points, long last_update);

	int GetRequestsLeft();

	static void ExecFreeOperation(std::string account_id, std::shared_ptr<Blockchain>blockchain,
		std::function<void()> on_success, std::function<void(std::string)> on_error);

	static void GetByAccountRateLimit(std::string id, std::shared_ptr<Blockchain> blockchain,
		std::function<void(std::shared_ptr<RateLimit>)> on_success, std::function<void(std::string)> on_error);

	static void GivePoints(std::string account_id, int points, std::shared_ptr<Blockchain> blockchain,
		std::function<void()> on_success, std::function<void(std::string)> on_error);

	static void GetLastTimestamp(std::shared_ptr<Blockchain> blockchain, std::function<void(long)> on_success, std::function<void(std::string)> on_error);

	static void GetPointsAvailable(int points, long last_operation, std::shared_ptr<Blockchain> blockchain,
		std::function<void(int)> on_success, std::function<void(std::string)> on_error);
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
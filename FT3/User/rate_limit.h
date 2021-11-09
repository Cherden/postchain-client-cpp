#pragma once

#include <memory>
#include <string>
#include <functional>
#include "../Core/Blockchain/blockchain.h"
#include "../User/account_dev_operations.h"

namespace chromia {
namespace postchain {
namespace ft3 {
class RateLimit
{
public:
	int points_;
    long last_update_;

	RateLimit(int points, long last_update)
    {
        points_ = points;
        last_update_ = last_update;
    }

    int GetRequestsLeft()
    {
        return points_;
    }

    static void ExecFreeOperation(std::string account_id, std::shared_ptr<Blockchain>blockchain, 
		std::function<void()> on_success, std::function<void(string)> on_error)
    {
		std::shared_ptr<TransactionBuilder> tx_builder = blockchain->TransactionBuilder();
		tx_builder->Add(AccountDevOperations::FreeOp(account_id));
		tx_builder->Add(AccountOperations::Nop());

		std::shared_ptr<ft3::Transaction> tx = tx_builder->Build(std::vector<std::vector<byte>>(), on_error);

		std::function<void()> on_success_wrapper = [on_success]() {
			on_success();
		};

		tx->PostAndWait(on_success_wrapper);
    }

    static void GetByAccountRateLimit(std::string id, std::shared_ptr<Blockchain> blockchain, 
		std::function<void(std::shared_ptr<RateLimit>)> on_success, std::function<void(string)> on_error)
    {
		std::vector<QueryObject> query_objects;
		query_objects.push_back(QueryObject("account_id", AbstractValueFactory::Build(id)));
		std::function<void(std::string)> on_success_wrapper = [on_success, on_error](std::string content) {
			// TO-DO check parsing
			nlohmann::json json_content = nlohmann::json::parse(content);
			if (json_content.contains("id") && json_content.contains("name") && json_content.contains("chain_id") && json_content.contains("amount"))
			{
				std::string amount_str = json_content["amount"];
				int points = atoi(std::string(json_content["amount"]).c_str());
				int last_update = atoi(std::string(json_content["last_updates"]).c_str());
				std::shared_ptr<RateLimit> rate_limit = std::make_shared<RateLimit>(points, last_update);
				on_success(rate_limit);
			}
			else
			{
				on_error("Assrt::GetById failed, corrupted resposne");
			}
		};

		blockchain->Query("ft3.get_account_rate_limit_last_update", query_objects, on_success_wrapper, on_error);
    }

	static void GivePoints(std::string account_id, int points, std::shared_ptr<Blockchain> blockchain, 
		std::function<void()> on_success, std::function<void(string)> on_error)
	{
		std::shared_ptr<TransactionBuilder> tx_builder = blockchain->TransactionBuilder();
		tx_builder->Add(AccountDevOperations::GivePoints(account_id, points));
		tx_builder->Add(AccountOperations::Nop());

		std::shared_ptr<ft3::Transaction> tx = tx_builder->Build(std::vector<std::vector<byte>>(), on_error);

		tx->PostAndWait(on_success);
	}

    static void GetLastTimestamp(std::shared_ptr<Blockchain> blockchain, std::function<void(long)> on_success, std::function<void(std::string)> on_error)
    {
		std::function<void(std::string)> on_success_wrapper = [on_success, on_error](std::string content) {
			// TO-DO check parsing
			//nlohmann::json json_content = nlohmann::json::parse(content);
			long timestamp = 0;
			on_success(timestamp);
		};

		blockchain->Query("ft3.get_last_timestamp", {}, on_success_wrapper, on_error);
    }

    static void GetPointsAvailable(int points, long last_operation, std::shared_ptr<Blockchain> blockchain,
        std::function<void(int)> on_success, std::function<void(string)> on_error)
    {
		int max_count = blockchain->info_->rate_limit_info_.max_points_;
		int recovery_time = blockchain->info_->rate_limit_info_.recovery_time_;
        long last_timestamp = 0;

		std::function<void(long)> on_receive_timestamp = [&last_timestamp, on_success, on_error, last_operation, recovery_time, points, max_count](long timestamp) {
			last_timestamp = timestamp;

			long delta = last_timestamp - last_operation;

			int points_available = (int)floor((double)delta / (double)recovery_time) + points;
			if (points_available > max_count)
			{
				on_success(max_count);
			}

			if (points_available > 0)
			{
				on_success(points_available);
			}
			else
			{
				on_success(0);
			}
		};
		
        GetLastTimestamp(blockchain, on_receive_timestamp, on_error);
    }
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
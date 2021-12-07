#pragma once

#include <string>
#include "../../PostchainClient/GTV/abstract_value_factory.h"
#include "../../PostchainClient/postchain_util.h"
#include "../../PostchainClient/common.h"
#include "../forward_declarations.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {
class Asset
{
public:
	std::string id_;
    std::string name_;

    std::string issuing_chain_rid_;

	Asset(std::string name, std::string chain_id);

	Asset(std::string id, std::string name, std::string issuing_chain_rid);

	static void Register(std::string name, std::string chain_id, std::shared_ptr<Blockchain> blockchain,
		std::function<void(std::shared_ptr<Asset>)> on_success,
		std::function<void(std::string)> on_error);

	static void GetByName(std::string name, std::shared_ptr<Blockchain> blockchain,
		std::function<void(std::vector<std::shared_ptr<Asset>>)> on_success, std::function<void(std::string)> on_error);

	static void GetById(std::string id, std::shared_ptr<Blockchain> blockchain,
		std::function<void(std::shared_ptr<Asset>)> on_success, std::function<void(std::string)> on_error);

	static void GetAssets(std::shared_ptr<Blockchain> blockchain,
		std::function<void(std::vector<std::shared_ptr<Asset>>)> on_success, std::function<void(std::string)> on_error);

private:
	std::string HashId();
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
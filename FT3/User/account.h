#pragma once

#include <vector>
#include <string>
#include "../../src/common.h"
#include "../../src/GTV/abstract_value_factory.h"
#include "../../src/GTV/array_value.h"
#include "../forward_declarations.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {

enum AuthType
{
	eAuthNone,
	eSingleSig,
	eMultiSig
};

enum FlagsType
{
	eFlagNone,
	eAccount,
	eTransfer
};

class Flags
{
public:
	std::vector<FlagsType> flag_list_;

	Flags(std::vector<FlagsType> flags);

	bool HasFlag(FlagsType flag);

	bool IsValid(FlagsType flag);

	std::shared_ptr<ArrayValue> ToGTV();
};

class Account
{
public:
	std::string id_;
    std::vector<std::shared_ptr<AuthDescriptor>> auth_descriptors_;
    std::vector<std::shared_ptr<AssetBalance>> assets_;
    std::shared_ptr<RateLimit> rate_limit_;
    std::shared_ptr<BlockchainSession> session_;

	Account(std::string id, std::vector<std::shared_ptr<AuthDescriptor>> auth_descriptors, std::shared_ptr<BlockchainSession> session);

	std::string GetID();

	std::shared_ptr<Blockchain> GetBlockchain();

	static void GetByParticipantId(std::string id, std::shared_ptr<BlockchainSession> session,
		std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error);

	static void GetByAuthDescriptorId(std::string id, std::shared_ptr<BlockchainSession> session,
		std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error);

	static void Register(std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<BlockchainSession> session,
		std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error);

	static std::vector<byte> RawTransactionRegister(std::shared_ptr<User> user, std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<Blockchain> blockchain);

	static std::vector<byte> RawTransactionAddAuthDescriptor(std::string account_id, std::shared_ptr<User> user,
		std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<Blockchain> blockchain);

	static void GetByIds(std::vector<std::string> ids, std::shared_ptr<BlockchainSession> session,
		std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error);

	static void GetById(std::string id, std::shared_ptr<BlockchainSession> session, std::function<void(shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error);

	void AddAuthDescriptor(std::shared_ptr<AuthDescriptor> auth_descriptor, std::function<void()> on_success, std::function<void(std::string)> on_error);

	void IsAuthDescriptorValid(std::string id, std::function<void(bool)> on_success, std::function<void(std::string)> on_error);

	void DeleteAllAuthDescriptorsExclude(std::shared_ptr<AuthDescriptor> auth_descriptor, std::function<void()> on_success, std::function<void(std::string)> on_error);

	void DeleteAuthDescriptor(std::shared_ptr<AuthDescriptor> auth_descriptor, std::function<void()> on_success, std::function<void(std::string)> on_error);

	void Sync(std::function<void()> on_success, std::function<void(std::string)> on_error);

	std::shared_ptr<AssetBalance> GetAssetById(std::string id);

	void TransferInputsToOutputs(std::shared_ptr<AbstractValue> inputs, std::shared_ptr<AbstractValue> outputs,
		std::function<void()> on_success, std::function<void(std::string)> on_error);

	void Transfer(std::string account_id, std::string asset_id, long amount,
		std::function<void()> on_success, std::function<void(std::string)> on_error);

	void BurnTokens(std::string asset_id, long amount, std::function<void()> on_success, std::function<void(std::string)> on_error);

	void XcTransfer(std::string destination_chain_id, std::string destination_account_id, std::string assetId, long amount,
		std::function<void()> on_success, std::function<void(std::string)> on_error);

	std::shared_ptr<ft3::Operation> XcTransferOp(std::string destination_chain_id, std::string destination_account_id, std::string asset_id, long amount);

private:
	void SyncAssets(std::function<void()> on_success, std::function<void(std::string)> on_error);

	void SyncAuthDescriptors(std::function<void()> on_success, std::function<void(std::string)> on_error);

	void SyncRateLimit(std::function<void()> on_success, std::function<void(std::string)> on_error);

};

} // namespace http
} // namespace postchain
} // namespace chromia
#include "account.h"
#include "../Core/util.h"
#include "asset_balance.h"
#include "rate_limit.h"
#include "../Core/Blockchain/blockchain.h"
#include "../Core/Blockchain/blockchain_session.h"

namespace chromia {
namespace postchain {
namespace ft3 {

Flags::Flags(std::vector<FlagsType> flags)
{
	this->flag_list_ = flags;
}

bool Flags::HasFlag(FlagsType flag)
{
	return std::find(flag_list_.begin(), flag_list_.end(), flag) != flag_list_.end();
}

bool Flags::IsValid(FlagsType flag)
{
	return flag == FlagsType::eAccount || flag == FlagsType::eTransfer;
}

std::shared_ptr<ArrayValue> Flags::ToGTV()
{
	std::shared_ptr<ArrayValue> valid_flags;
	for (FlagsType &flag : this->flag_list_)
	{
		if (IsValid(flag))
		{
			std::string as_string = FT3Util::FlagTypeToString(flag);
			valid_flags->Add(AbstractValueFactory::Build(as_string));
		}
	}
	return valid_flags;
}

Account::Account(std::string id, std::vector<std::shared_ptr<AuthDescriptor>> auth_descriptors, std::shared_ptr<BlockchainSession> session)
{
	this->id_ = id;
	this->auth_descriptors_ = auth_descriptors;
	this->session_ = session;
}

std::string Account::GetID()
{
	return this->id_;
}

std::shared_ptr<Blockchain> Account::GetBlockchain()
{
	return this->session_->blockchain_;
}

void Account::GetByParticipantId(std::string id, std::shared_ptr<BlockchainSession> session,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error)
{

}

//void Account::GetByParticipantId(std::string id, std::shared_ptr<BlockchainSession> session,
//	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error) 
//{}
/*{
	std::vector<std::string> account_ids;
	yield return session.Query<string[]>("ft3.get_accounts_by_participant_id", new (string, object)[] { ("id", id) },
		(string[] _accountIDs) => accountIDs = _accountIDs, onError);

	yield return Account.GetByIds(accountIDs, session, onSuccess, onError);
}*/

void Account::GetByAuthDescriptorId(std::string id, std::shared_ptr<BlockchainSession> session,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error)
{}
/* {
	 string[] accountIDs = null;
	 yield return session.Query<string[]>("ft3.get_accounts_by_auth_descriptor_id", new (string, object)[] { ("descriptor_id", id) },
		 (string[] _accountIDs) => accountIDs = _accountIDs, onError);

	 yield return Account.GetByIds(accountIDs, session, onSuccess, onError);
 }*/

void Account::Register(std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<BlockchainSession> session,
	std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
{}


std::vector<byte> Account::RawTransactionRegister(std::shared_ptr<User> user, std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<Blockchain> blockchain)
{
	return {};
}
/* {
var signers = new List<byte[]>();
signers.AddRange(user.AuthDescriptor.Signers);
signers.AddRange(authDescriptor.Signers);

return blockchain.TransactionBuilder()
	.Add(AccountDevOperations.Register(user.AuthDescriptor))
	.Add(AccountOperations.AddAuthDescriptor(user.AuthDescriptor.ID, user.AuthDescriptor.ID, authDescriptor))
	.Build(signers.ToArray(), null)
	.Sign(user.KeyPair)
	.Raw();
}*/

std::vector<byte> Account::RawTransactionAddAuthDescriptor(std::string account_id, std::shared_ptr<User> user,
	std::shared_ptr<AuthDescriptor> auth_descriptor, std::shared_ptr<Blockchain> blockchain)
{
	return {};
}
/* {
	 var signers = new List<byte[]>();
	 signers.AddRange(user.AuthDescriptor.Signers);
	 signers.AddRange(authDescriptor.Signers);

	 return blockchain.TransactionBuilder()
		 .Add(AccountOperations.AddAuthDescriptor(user.AuthDescriptor.ID, user.AuthDescriptor.ID, authDescriptor))
		 .Build(signers.ToArray(), null)
		 .Sign(user.KeyPair)
		 .Raw();
 }*/

void Account::GetByIds(std::vector<std::string> ids, std::shared_ptr<BlockchainSession> session,
	std::function<void(std::vector<std::shared_ptr<Account>>)> on_success, std::function<void(string)> on_error)
{

}
/* {
	 var accounts = new List<Account>();
	 foreach (var id in ids)
	 {
		 yield return Account.GetById(id, session, (Account account) => accounts.Add(account), onError);
	 }

	 onSuccess(accounts.ToArray());
 }*/


void Account::GetById(std::string id, std::shared_ptr<BlockchainSession> session, std::function<void(Account)> on_success, std::function<void(std::string)> on_error)
{

}


void Account::AddAuthDescriptor(std::shared_ptr<AuthDescriptor> authDescriptor, std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
/*  {
	  yield return this.Session.Call(AccountOperations.AddAuthDescriptor(
		  this.Id,
		  this.Session.User.AuthDescriptor.ID,
		  authDescriptor),
		  () =>
		  {
			  this.AuthDescriptor.Add(authDescriptor);
			  onSuccess();
		  }, onError
	  );
  }*/

void Account::IsAuthDescriptorValid(std::string id, std::function<void(bool)> on_success, std::function<void(std::string)> on_error)
{

}
/*{
	yield return Session.Query<bool>("ft3.is_auth_descriptor_valid",
		new (string, object)[] { ("account_id", this.Id), ("auth_descriptor_id", Util.HexStringToBuffer(id)) },
		onSuccess,
		onError
	);
}*/

void Account::DeleteAllAuthDescriptorsExclude(std::shared_ptr<AuthDescriptor> authDescriptor, std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
/* {
	 yield return this.Session.Call(AccountOperations.DeleteAllAuthDescriptorsExclude(
		 this.Id,
		 authDescriptor.ID),
		 () =>
		 {
			 this.AuthDescriptor.Clear();
			 this.AuthDescriptor.Add(authDescriptor);
			 onSuccess();
		 }, onError
	 );
 }*/

void Account::DeleteAuthDescriptor(std::shared_ptr<AuthDescriptor> authDescriptor, std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
//{
//    yield return this.Session.Call(AccountOperations.DeleteAuthDescriptor(
//        this.Id,
//        this.Session.User.AuthDescriptor.ID,
//        authDescriptor.ID),
//        () =>
//        {
//            this.AuthDescriptor.Remove(authDescriptor);
//            onSuccess();
//        }, onError
//    );
//}

void Account::Sync(std::function<void()> on_success, std::function<void(std::string)> on_error)
{

	/* yield return SyncAssets(() => { }, onError);
	 yield return SyncAuthDescriptors(() => { }, onError);
	 yield return SyncRateLimit(() => { }, onError);

	 onSuccess();*/
}

void Account::SyncAssets(std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
//{
//    yield return AssetBalance.GetByAccountId(this.Id, this.Session.Blockchain,
//        (AssetBalance[] balances) =>
//        {
//            this.Assets = balances.ToList();
//            onSuccess();
//        }, onError
//    );
//}

void Account::SyncAuthDescriptors(std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
/*{
	AuthDescriptorFactory.AuthDescriptorQuery[] authDescriptors = null;

	yield return this.Session.Query<AuthDescriptorFactory.AuthDescriptorQuery[]>("ft3.get_account_auth_descriptors",
		new (string, object)[] { ("id", this.Id) },
		(AuthDescriptorFactory.AuthDescriptorQuery[] authQuery) =>
		{
			authDescriptors = authQuery;
		}, onError);

	var authDescriptorFactory = new AuthDescriptorFactory();
	List<AuthDescriptor> authList = new List<AuthDescriptor>();

	foreach (var authDescriptor in authDescriptors)
	{
		authList.Add(
			authDescriptorFactory.Create(
				Util.StringToAuthType((string)authDescriptor.type),
				Util.HexStringToBuffer((string)authDescriptor.args)
			)
		);
	}

	this.AuthDescriptor = authList;
	onSuccess();
}*/

void Account::SyncRateLimit(std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
/*{
	yield return RateLimit.GetByAccountRateLimit(this.Id, this.Session.Blockchain,
		(RateLimit rateLimit) =>
		{
			this.RateLimit = rateLimit;
			onSuccess();
		}, onError
	);
}*/

std::shared_ptr<AssetBalance> Account::GetAssetById(std::string id)
{
	return nullptr;
}
//{
//    return this.Assets.Find(assetBalance => assetBalance.Asset.Id.Equals(id));
//}

void Account::TransferInputsToOutputs(std::shared_ptr<AbstractValue> inputs, std::shared_ptr<AbstractValue> outputs,
	std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
/* {
		bool isTransferCompleted = false;

		yield return this.Session.Call(AccountOperations.Transfer(inputs, outputs),
			() => isTransferCompleted = true, onError);

		if (isTransferCompleted)
		{
			yield return this.SyncAssets(onSuccess, onError);
		}
	}*/

void Account::Transfer(std::string account_id, std::string asset_id, long amount,
	std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
//{
//    var input = new List<object>{
//        this.Id,
//        assetId,
//        this.Session.User.AuthDescriptor.ID,
//        amount,
//        new object[] {}
//    }.ToArray();

//    var output = new List<object>{
//        accountId,
//        assetId,
//        amount,
//        new object[] {}
//    }.ToArray();

//    yield return this.TransferInputsToOutputs(
//        new object[] { input },
//        new object[] { output },
//        onSuccess, onError
//    );
//}

void Account::BurnTokens(std::string assetId, long amount, std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
//{
	/* var input = new List<object>(){
		this.Id,
		assetId,
		this.Session.User.AuthDescriptor.Hash(),
		amount,
		new object[] {}
	}.ToArray();

	yield return this.TransferInputsToOutputs(
		new List<object>() { input }.ToArray(),
		new List<object>() { }.ToArray(),
		onSuccess, onError
	);*/
	// }

void Account::XcTransfer(std::string destination_chain_id, std::string destination_account_id, std::string assetId, long amount,
	std::function<void()> on_success, std::function<void(std::string)> on_error)
{

}
/*{
	yield return this.Session.Call(this.XcTransferOp(
		destinationChainId, destinationAccountId, assetId, amount),
		() =>
		{
			this.SyncAssets(onSuccess, onError);
		}, onError
	);
}*/

std::shared_ptr<ft3::Operation> Account::XcTransferOp(std::string destination_chain_id, std::string destinationAccountId, std::string asset_id, long amount)
{
	return nullptr;
}
/*{
	var source = new object[] {
		this.Id,
		assetId,
		this.Session.User.AuthDescriptor.ID,
		amount,
		new object[]{}
	};

	var target = new object[] {
		destinationAccountId,
		new object[]{}
	};

	var hops = new string[] {
		destinationChainId
	};

	return AccountOperations.XcTransfer(source, target, hops);
}*/

} // namespace http
} // namespace postchain
} // namespace chromi
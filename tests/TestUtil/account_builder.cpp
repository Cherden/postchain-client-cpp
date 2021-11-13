//#ifdef FT3_TESTS

#include "account_builder.h"

#include "CoreMinimal.h" // TO-DO get rid of UE4 dependencies
#include "../../../ChromaUnreal/Utils.h"
#include "../../FT3/User/account.h"
#include "../../FT3/User/user.h"
#include "../../FT3/Core/key_pair.h"
#include "../../src/common.h"

AccountBuilder::AccountBuilder(std::shared_ptr<Blockchain> blockchain, std::shared_ptr<User> user)
{
	std::shared_ptr <User> new_user = user;
	if (user == nullptr)
	{
		new_user = TestUser::SingleSig();
	}

	this->blockchain_ = blockchain;
	this->participants_ = { new_user->key_pair_ };
	this->user_ = new_user;
}

std::shared_ptr<AccountBuilder> AccountBuilder::CreateAccountBuilder(std::shared_ptr<Blockchain> blockchain, std::shared_ptr<User> user = nullptr)
{
	return std::make_shared<AccountBuilder>(blockchain, user);
}

std::shared_ptr<AccountBuilder> AccountBuilder::WithAuthFlags(std::vector<FlagsType> flags)
{
	this->flags_.clear();
	for (FlagsType flag : flags)
	{
		this->flags_.push_back(flag);
	}
	return std::shared_ptr<AccountBuilder>(this);
}

std::shared_ptr<AccountBuilder> AccountBuilder::WithParticipants(std::vector<std::shared_ptr<KeyPair>> participants)
{
	this->participants_.clear();
	for (auto &item : participants)
	{
		this->participants_.push_back(item);
	}

	return std::shared_ptr<AccountBuilder>(this);
}

std::shared_ptr<AccountBuilder> AccountBuilder::WithBalance(std::shared_ptr<Asset> asset, int balance)
{
	this->asset_ = asset;
	this->balance_ = balance;
	return std::shared_ptr<AccountBuilder>(this);
}

std::shared_ptr<AccountBuilder> AccountBuilder::WithPoints(int points)
{
	this->points_ = points;
	return std::shared_ptr<AccountBuilder>(this);
}

std::shared_ptr<AccountBuilder> AccountBuilder::WithRequiredSignatures(int count)
{
	this->required_signatures_count_ = count;
	return std::shared_ptr<AccountBuilder>(this);
}


void AccountBuilder::DefaultErrorHandler(std::string error)
{
	UE_LOG(LogTemp, Error, TEXT("FT3:: [%s]"), *(ChromaUtils::STDStringToFString(error)));
}

//#endif // FT3_TESTS
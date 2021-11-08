/**
* cpp - http wrapper.
* this header is UE4 independent.
* TO-DO replace implementation with pure openssl lib, to make it UE4 full independent
*/
#pragma once

#include <string>
#include <functional>

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "CoreMinimal.h"

#include "httprequest.generated.h"

using namespace std;

//namespace chromia {
//namespace postchain {
//namespace http {
//
UCLASS()
class UHttpRequest : public UObject
{
	GENERATED_BODY()

public:

	UHttpRequest(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {};

	static std::shared_ptr<UHttpRequest> BuildHttpRequest(std::string url, 
		std::function<void(std::string)> success_callback, 
		std::function<void(std::string)> error_callback);

	void SetContent(std::string content);

	void Get();

	void Post();

private:

	std::string url_;
	std::string content_;
	std::function<void(std::string)> success_callback_;
	std::function<void(std::string)> error_callback_;

	///*Called when the server has responded to http request*/
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

//} // namespace http
//} // namespace postchain
//} // namespace chromia
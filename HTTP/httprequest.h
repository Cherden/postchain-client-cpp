/**
* cpp - http wrapper.
* this header is UE4 independent.
* TO-DO replace implementation with pure openssl lib, to make it UE4 full independent
*/
#pragma once

#include <string>
#include <functional>

#include "HttpModule.h"
#include "HttpManager.h"
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
	static void SendPostRequest(std::string url, std::string payload,
		std::function<void(std::string)> success_callback, 
		std::function<void(std::string)> error_callback);

	static void SendGetRequestSync(std::string url, std::string &content, std::string &error);
private:
	static bool response_success;
	static std::string response_content;

	/* Called when the server has responded to http post request */
	static void OnSyncPostResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/* Called when the server has responded to http get request */
	static void OnSyncGetResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

//} // namespace http
//} // namespace postchain
//} // namespace chromia
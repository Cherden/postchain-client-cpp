// put this at the top of your .h file above #includes
// UE4: allow Windows platform types to avoid naming collisions
// must be undone at the bottom of this file!
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include "httprequest.h"
#include "../../ChromaUnreal/Utils.h"
#include "../src/postchain_util.h"

#include <string>      
#include <iostream>   
#include <sstream> 

#include <curl/curl.h>

bool UHttpRequest::response_success = false;

std::string UHttpRequest::response_content = "{}";

size_t UHttpRequest::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


void UHttpRequest::SendPostRequest(std::string url, std::string payload,
	std::function<void(std::string)> success_callback, std::function<void(std::string)> error_callback)
{
	CURL *curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		
		try
		{
			curl_easy_cleanup(curl);
			std::cout << readBuffer << std::endl;
		}
		catch (std::exception e)
		{
			error_callback(std::string(e.what()));
		}
	}
	else
	{
		error_callback(std::string("curl_easy_init() failed"));
	}

	// Wait for request to finish
	UE_LOG(LogTemp, Error, TEXT("SendPostRequest url: %s"), *ChromaUtils::STDStringToFString(url));
	UE_LOG(LogTemp, Error, TEXT("SendPostRequest response: %s"), *ChromaUtils::STDStringToFString(readBuffer));

	success_callback(readBuffer);

	//FString furl = ChromaUtils::STDStringToFString(url);
	//FString fcontent = ChromaUtils::STDStringToFString(payload);

	//TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	//Request->OnProcessRequestComplete().BindStatic(&UHttpRequest::OnSyncPostResponseReceived);
	//Request->SetURL(*furl);
	//Request->SetVerb("POST");
	//Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	//Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	//Request->SetContentAsString(*fcontent);

	////Send the request
	//bool request_started = Request->ProcessRequest();
	//if (!request_started)
	//{
	//	error_callback("UHttpRequest::POST request->ProcessRequest() failed");
	//	return;
	//}

	//// Wait for request to finish
	//FHttpModule::Get().GetHttpManager().Flush(false);
	//Request->CancelRequest();

	//if (response_success)
	//{
	//	nlohmann::json json_content = nlohmann::json::parse(response_content);
	//	std::string error_message = postchain::PostchainUtil::GetSafeJSONString(json_content, "error", "");
	//	if (error_message.size() == 0)
	//	{
	//		success_callback(response_content);
	//	}
	//	else
	//	{
	//		error_callback(error_message);
	//	}
	//}
	//else
	//{
	//	error_callback(response_content);
	//}
}


void UHttpRequest::SendGetRequestSync(std::string url, std::string &content, std::string &error)
{
	CURL *curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);

		try 
		{
			curl_easy_cleanup(curl);
			std::cout << readBuffer << std::endl;
		}
		catch (std::exception e)
		{
			error = std::string(e.what());
		}
	}
	else 
	{
		error = "curl_easy_init() failed";
	}

	content = readBuffer;

	UE_LOG(LogTemp, Error, TEXT("SendGetRequestSync url: %s"), *ChromaUtils::STDStringToFString(url));
	UE_LOG(LogTemp, Error, TEXT("SendGetRequestSync response: %s"), *ChromaUtils::STDStringToFString(content));

	/*content = readBuffer;
	UE_LOG(LogTemp, Error, TEXT("Result: %s"), *ChromaUtils::STDStringToFString(readBuffer));*/

	//FString furl = ChromaUtils::STDStringToFString(url);

	//TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	//Request->OnProcessRequestComplete().BindStatic(&UHttpRequest::OnSyncGetResponseReceived);
	//Request->SetURL(*furl);
	//Request->SetVerb("GET");
	//Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	//Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	////Send the request
	//bool request_started = Request->ProcessRequest();
	//if (!request_started)
	//{
	//	error = "UHttpRequest::GET request->ProcessRequest() failed";
	//	return;
	//}

	//// Wait for request to finish
	//FHttpModule::Get().GetHttpManager().Flush(false);
	//Request->CancelRequest();

	//if (response_success)
	//{
	//	nlohmann::json json_content = nlohmann::json::parse(response_content);
	//	std::string error_message = postchain::PostchainUtil::GetSafeJSONString(json_content, "error", "");
	//	if (error_message.size() == 0)
	//	{
	//		content = response_content;
	//		return;
	//	}
	//	else
	//	{
	//		error = error_message;
	//		return;
	//	}
	//}
	//else
	//{
	//	error = response_content;
	//	return;
	//}
}

void UHttpRequest::OnSyncPostResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		response_success = true;
		response_content = ChromaUtils::FStringToSTDString(Response->GetContentAsString());
	}
	else
	{
		response_success = false;
		response_content = "UHttpRequest::POST request failed";
	}
}

void UHttpRequest::OnSyncGetResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		response_success = true;
		response_content = ChromaUtils::FStringToSTDString(Response->GetContentAsString());
	}
	else
	{
		response_success = false;
		response_content = "UHttpRequest::GET request failed";
	}
}

// UE4: disallow windows platform types
// this was enabled at the top of the file
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
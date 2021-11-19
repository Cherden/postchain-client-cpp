#include "httprequest.h"
#include "../../ChromaUnreal/Utils.h"
#include "../src/postchain_util.h"

bool UHttpRequest::response_success = false;

std::string UHttpRequest::response_content = "{}";

void UHttpRequest::SendPostRequest(std::string url, std::string payload,
	std::function<void(std::string)> success_callback, std::function<void(std::string)> error_callback)
{
	FString furl = ChromaUtils::STDStringToFString(url);
	FString fcontent = ChromaUtils::STDStringToFString(payload);

	//UE_LOG(LogTemp, Display, TEXT("CHROMA::POST SendPostRequest(): url:     [%s]"), *furl);
	//UE_LOG(LogTemp, Display, TEXT("CHROMA::POST SendPostRequest(): payload: [%s]"), *fcontent);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindStatic(&UHttpRequest::OnSyncPostResponseReceived);
	Request->SetURL(*furl);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(*fcontent);

	//Send the request
	bool request_started = Request->ProcessRequest();
	if (!request_started)
	{
		error_callback("UHttpRequest::POST request->ProcessRequest() failed");
		return;
	}

	// Wait for request to finish
	FHttpModule::Get().GetHttpManager().Flush(false);
	Request->CancelRequest();

	UE_LOG(LogTemp, Display, TEXT("CHROMA::POST resposne:  [%s]"), *(ChromaUtils::STDStringToFString(response_content)));

	if (response_success)
	{
		nlohmann::json json_content = nlohmann::json::parse(response_content);
		std::string error_message = postchain::PostchainUtil::GetSafeJSONString(json_content, "error", "");
		if (error_message.size() == 0)
		{
			success_callback(response_content);
		}
		else
		{
			error_callback(error_message);
		}
	}
	else
	{
		error_callback(response_content);
	}
}


void UHttpRequest::SendGetRequestSync(std::string url, std::string &content, std::string &error)
{
	FString furl = ChromaUtils::STDStringToFString(url);

	//UE_LOG(LogTemp, Display, TEXT("CHROMA:: GET url:     [%s]"), *furl);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindStatic(&UHttpRequest::OnSyncGetResponseReceived);
	Request->SetURL(*furl);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	//Send the request
	bool request_started = Request->ProcessRequest();
	if (!request_started)
	{
		error = "UHttpRequest::GET request->ProcessRequest() failed";
		return;
	}

	// Wait for request to finish
	FHttpModule::Get().GetHttpManager().Flush(false);
	Request->CancelRequest();

	//UE_LOG(LogTemp, Display, TEXT("CHROMA::GET resposne: status %d [%s]"), response_success, *(ChromaUtils::STDStringToFString(response_content)));

	if (response_success)
	{
		nlohmann::json json_content = nlohmann::json::parse(response_content);
		std::string error_message = postchain::PostchainUtil::GetSafeJSONString(json_content, "error", "");
		if (error_message.size() == 0)
		{
			content = response_content;
			return;
		}
		else
		{
			error = error_message;
			return;
		}
	}
	else
	{
		error = response_content;
		return;
	}
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
#include "httprequest.h"
#include "../../ChromaUnreal/Utils.h"

bool UHttpRequest::response_success = false;

std::string UHttpRequest::response_content = "{}";

void UHttpRequest::SendPostRequest(std::string url, std::string payload,
	std::function<void(std::string)> success_callback, std::function<void(std::string)> error_callback)
{
	FString furl = ChromaUtils::STDStringToFString(url);
	FString fcontent = ChromaUtils::STDStringToFString(payload);

	UE_LOG(LogTemp, Error, TEXT("CHROMA::SendPostRequest(): url:     [%s]"), *furl);
	UE_LOG(LogTemp, Error, TEXT("CHROMA::SendPostRequest(): payload: [%s]"), *fcontent);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindStatic(&UHttpRequest::OnSyncResponseReceived);
	Request->SetURL(*furl);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(*fcontent);

	//Send the request
	bool request_started = Request->ProcessRequest();
	if (!request_started)
	{
		error_callback("UHttpRequest::request->ProcessRequest() failed");
		return;
	}

	// Wait for request to finish
	FHttpModule::Get().GetHttpManager().Flush(false);
	Request->CancelRequest();

	UE_LOG(LogTemp, Error, TEXT("CHROMA::SendPostRequest(): resposne: status %d [%s]"), response_success, *(ChromaUtils::STDStringToFString(response_content)));

	if (response_success)
	{
		success_callback(response_content);
	}
	else
	{
		error_callback(response_content);
	}
}

void UHttpRequest::OnSyncResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Error, TEXT("CHROMA::OnSyncResponseReceived(): %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("CHROMA::OnSyncResponseReceived(): %s"), *(Response->GetContentAsString()));
		response_success = true;
		response_content = ChromaUtils::FStringToSTDString(Response->GetContentAsString());
	}
	else
	{
		response_success = false;
		response_content = "UHttpRequest::request failed";
	}
}

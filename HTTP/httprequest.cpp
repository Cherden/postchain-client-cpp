#include "httprequest.h"
#include "../../ChromaUnreal/Utils.h"

std::shared_ptr<UHttpRequest> UHttpRequest::BuildHttpRequest(std::string url, 
	std::function<void(int, std::string)> success_callback, std::function<void(int, std::string)> error_callback)
{
	std::shared_ptr<UHttpRequest> request(NewObject<UHttpRequest>());
	request->url_ = url;
	request->success_callback_ = success_callback;
	request->error_callback_ = error_callback;

	return request;
}


void UHttpRequest::SetContent(std::string content)
{
	this->content_ = content;
}


void UHttpRequest::Get()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UHttpRequest::OnResponseReceived);
	request->SetURL(*(ChromaUtils::STDStringToFString(this->url_)));
	request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	request->SetVerb("GET");
	request->SetContentAsString(*(ChromaUtils::STDStringToFString(this->content_)));
	request->ProcessRequest();
}


void UHttpRequest::Post()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UHttpRequest::OnResponseReceived);
	request->SetURL(*(ChromaUtils::STDStringToFString(this->url_)));
	request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	request->SetVerb("POST");
	request->SetContentAsString(*(ChromaUtils::STDStringToFString(this->content_)));
	request->ProcessRequest();
}


void UHttpRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful){
		success_callback_(Response->GetResponseCode(), ChromaUtils::FStringToSTDString(Response->GetContentAsString()));
	}
	else 
	{
		error_callback_(Response->GetResponseCode(), "UHttpRequest Failed");
	}
}

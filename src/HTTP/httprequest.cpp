#ifdef CHROMIA_INSIDE_UNREAL_ENGINE
// put this at the top of your .h file above #includes
// UE4: allow Windows platform types to avoid naming collisions
// must be undone at the bottom of this file!
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"
#endif // CHROMIA_INSIDE_UNREAL_ENGINE

#include "httprequest.h"
#include "../PostchainClient/postchain_util.h"

#include <string>       
#include <sstream> 

#ifdef CHROMIA_INSIDE_UNREAL_ENGINE
	#define HTTP_REQUEST_TIMEOUT_S 5
	#define HTTP_REQUEST_LOOP_DELAY_S 0.01
	#include "Interfaces/IHttpRequest.h"
	#include "HttpModule.h"
	//#include <Curl/curl.h>
	#include "../../../ChromaUnreal/ChromaUtils.h"
#else
	#include <curl/curl.h>
#endif // CHROMIA_INSIDE_UNREAL_ENGINE

namespace chromia {
namespace postchain {
namespace http {

size_t HttpRequest::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


void HttpRequest::SendPostRequest(std::string url, std::string payload,
	std::function<void(std::string)> success_callback, std::function<void(std::string)> error_callback)
{
#ifdef CHROMIA_INSIDE_UNREAL_ENGINE
	UE_LOG(LogTemp, Error, TEXT("CHROMA::HttpRequest::SendPostRequest: [%s]"), *ChromaUtils::STDStringToFString(url));

	bool requestFinished = false;
	//bool requestWasSuccessfull = false;
	//FString responseStr;
	//
	// Create the Callback when the HTTP-Request has finished
	auto OnRequestComplete = [&requestFinished] (FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
		UE_LOG(LogTemp, Error, TEXT("CHROMA::HttpRequest::OnRequestComplete"));
		//if (bWasSuccessful) {
		//	requestWasSuccessfull = true;
		//	responseStr = Response->GetContentAsString();
		//	success_callback(ChromaUtils::FStringToSTDString(content));
		//}
		//else 
		//{
		//	requestWasSuccessfull = false;
		//	responseStr = "SendPostRequest failed with code"; // FString::Printf(TEXT("SendPostRequest failed with code: %d"), Response->GetResponseCode());
		//	error_callback(ChromaUtils::FStringToSTDString(result));
		//}
		requestFinished = true;
	};

	// Create a HTTP POST Request 
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("POST");
	Request->SetURL(ChromaUtils::STDStringToFString(url));
	Request->SetContentAsString(ChromaUtils::STDStringToFString(payload));
	Request->OnProcessRequestComplete().BindLambda(OnRequestComplete);
	Request->SetTimeout(HTTP_REQUEST_TIMEOUT_S);
	
	bool bStartedRequest = Request->ProcessRequest();

	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::sleep begin"));
	FPlatformProcess::Sleep(0.5);
	Request->Tick(0.5);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::request was done: %d"), requestFinished);
	FPlatformProcess::Sleep(0.5);
	Request->Tick(0.5);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::request was done: %d"), requestFinished);
	FPlatformProcess::Sleep(0.5);
	Request->Tick(0.5);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::request was done: %d"), requestFinished);
	FPlatformProcess::Sleep(0.5);
	Request->Tick(0.5);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::request was done: %d"), requestFinished);
	FPlatformProcess::Sleep(0.5);
	Request->Tick(0.5);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::request was done: %d"), requestFinished);
	FPlatformProcess::Sleep(0.5);
	Request->Tick(0.5);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::request was done: %d"), requestFinished);
	FPlatformProcess::Sleep(0.5);
	Request->Tick(0.5);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::request was done: %d"), requestFinished);
	FPlatformProcess::Sleep(0.5);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::request was done: %d"), requestFinished);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::sleep done"));

	//bool wait = true; 	
	//while (wait) {
	//	FHttpResponsePtr Response = Request->GetResponse();
	//	if (Response.IsValid())
	//	{
	//		if (Response->GetContentAsString() != "")
	//		{
	//			break;
	//		}
	//	}
	//}

	//if (!bStartedRequest)
	//{
	//	error_callback(std::string("SendPostRequest failed to start HTTP POST Request"));
	//	return;
	//}

	//float elapsed = 0;
	//while (elapsed < HTTP_REQUEST_TIMEOUT_S)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::WAITING"));

	//	FHttpResponsePtr Response = Request->GetResponse();
	//	if (Response.IsValid())
	//	{
	//		int32 code = Response->GetResponseCode();
	//		FString content = Response->GetContentAsString();
	//		EHttpRequestStatus::Type status = Request->GetStatus();

	//		UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::Update code: [%d]  status: [%d] content: [%s] "), code, status, *content);

	//		/*if (content.Len() > 0)
	//		{
	//			return;
	//		}*/

	//		/*if (Code == 0)
	//		{
	//			FString content = Response->GetContentAsString();
	//			UE_LOG(LogTemp, Error, TEXT("CHROMA::HttpRequest::SendPostRequest RESULT: [%s]"), *content);
	//			if (content.Len() == 0)
	//			{
	//				error_callback(std::string("SendPostRequest failed"));
	//			}
	//			else
	//			{
	//				success_callback(ChromaUtils::FStringToSTDString(content));
	//			}
	//		}
	//		else
	//		{
	//			FString content = FString::Printf(TEXT("SendPostRequest failed with code: %d"), Code);
	//			error_callback(ChromaUtils::FStringToSTDString(content));
	//		}*/

	//		//return;
	//	}// else keep waiting

	//	FPlatformProcess::Sleep(HTTP_REQUEST_LOOP_DELAY_S);
	//	elapsed += HTTP_REQUEST_LOOP_DELAY_S;
	//}

	// Request timed out
	//error_callback("HTTP POST Request timed out");

#else // CHROMIA_INSIDE_UNREAL_ENGINE

	CURL *curl;
	CURLcode res;
	std::string read_buffer;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L); //0 disable messages

		res = curl_easy_perform(curl);
		
		try
		{
			curl_easy_cleanup(curl);
		}
		catch (std::exception e)
		{
			error_callback(std::string(e.what()));
		}
	}
	else
	{
		error_callback(std::string("SendPostRequest failed"));
	}

	if (read_buffer.size() == 0)
	{
		error_callback(std::string("SendPostRequest failed"));
	}
	else 
	{
		success_callback(read_buffer);
	}
#endif // CHROMIA_INSIDE_UNREAL_ENGINE
}


void HttpRequest::SendGetRequestSync(std::string url, std::string &content, std::string &error)
{
#ifdef CHROMIA_INSIDE_UNREAL_ENGINE
	UE_LOG(LogTemp, Error, TEXT("CHROMA::HttpRequest::SendGetRequestSync: [%s]"), *ChromaUtils::STDStringToFString(url));

	// Create the Callback when the HTTP-Request has finished
	/*auto OnRequestComplete = [&error](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
		if (bWasSuccessful) {
			FString content = Response->GetContentAsString();
			error = ChromaUtils::FStringToSTDString(content);
		}
		else
		{
			FString result = FString::Printf(TEXT("SendPostRequest failed with code: %d"), Response->GetResponseCode());
			error = ChromaUtils::FStringToSTDString(result);
		}
	};*/

	// Create a HTTP POST Request 
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(ChromaUtils::STDStringToFString(url));
	Request->SetContentAsString(ChromaUtils::STDStringToFString(content));
	Request->SetTimeout(HTTP_REQUEST_TIMEOUT_S);

	//Request->OnProcessRequestComplete().BindLambda(OnRequestComplete);

	bool bStartedRequest = Request->ProcessRequest();

	if (!bStartedRequest)
	{
		error = "SendGetRequestSync failed to start HTTP GET Request";
		return;
	}

	float elapsed = 0;
	while (elapsed < HTTP_REQUEST_TIMEOUT_S)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::HttpRequest::WAITING"));

		FHttpResponsePtr Response = Request->GetResponse();
		if (Response.IsValid())
		{
			int32 Code = Response->GetResponseCode();

			if (Code == 0)
			{
				//Success
				content = ChromaUtils::FStringToSTDString(Response->GetContentAsString());
				UE_LOG(LogTemp, Error, TEXT("CHROMA::HttpRequest::SendGetRequestSync RESULT: [%s]"), *Response->GetContentAsString());
			}
			else

			{
				error = "SendGetRequestSync failed";
			}

			return;
		}// else keep waiting

		FPlatformProcess::Sleep(HTTP_REQUEST_LOOP_DELAY_S);
		elapsed += HTTP_REQUEST_LOOP_DELAY_S;
	}

	// Timeout
	error = "SendGetRequestSync failed with timeout";

#else // CHROMIA_INSIDE_UNREAL_ENGINE

	//CURL *curl;
	//CURLcode res;
	//std::string read_buffer;

	//curl = curl_easy_init();
	//if (curl) 
	//{
	//	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
	//	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L); //0 disable messages
	//	res = curl_easy_perform(curl);

	//	try 
	//	{
	//		curl_easy_cleanup(curl);
	//	}
	//	catch (std::exception e)
	//	{
	//		error = std::string(e.what());
	//	}
	//}
	//else 
	//{
	//	error = "SendGetRequestSync failed";
	//}

	//content = read_buffer;
#endif // CHROMIA_INSIDE_UNREAL_ENGINE
}

} // namespace http
} // namespace postchain
} // namespace chromia

#ifdef CHROMIA_INSIDE_UNREAL_ENGINE
// UE4: disallow windows platform types
// this was enabled at the top of the file
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif // CHROMIA_INSIDE_UNREAL_ENGINE
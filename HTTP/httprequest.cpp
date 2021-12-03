// put this at the top of your .h file above #includes
// UE4: allow Windows platform types to avoid naming collisions
// must be undone at the bottom of this file!
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include "httprequest.h"
#include "../src/postchain_util.h"

#include <string>      
#include <iostream>   
#include <sstream> 

#include <curl/curl.h>

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
		error_callback(std::string("SendPostRequest failed"));
	}

	success_callback(readBuffer);
}


void HttpRequest::SendGetRequestSync(std::string url, std::string &content, std::string &error)
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
		error = "SendGetRequestSync failed";
	}

	content = readBuffer;
}

} // namespace http
} // namespace postchain
} // namespace chromia

// UE4: disallow windows platform types
// this was enabled at the top of the file
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
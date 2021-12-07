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
	std::string read_buffer;

	std::cout << "post url: " << url << "\n";
	std::cout << "post payload: " << payload << "\n";

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

	std::cout << "post response: " << read_buffer << "\n";

	success_callback(read_buffer);
}


void HttpRequest::SendGetRequestSync(std::string url, std::string &content, std::string &error)
{
	std::cout << "get url: " << url << "\n";

	CURL *curl;
	CURLcode res;
	std::string read_buffer;

	curl = curl_easy_init();
	if (curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
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
			error = std::string(e.what());
		}
	}
	else 
	{
		error = "SendGetRequestSync failed";
	}

	std::cout << "get response: " << read_buffer << "\n";
	content = read_buffer;
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
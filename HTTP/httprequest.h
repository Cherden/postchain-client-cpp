/**
* cpp - http wrapper.
* this header is UE4 independent.
* TO-DO replace implementation with pure openssl lib, to make it UE4 full independent
*/
#pragma once

#include <string>
#include <functional>

using namespace std;

namespace chromia {
namespace postchain {
namespace http {


class HttpRequest
{

public:
	static void SendPostRequest(std::string url, std::string payload,
		std::function<void(std::string)> success_callback, 
		std::function<void(std::string)> error_callback);

	static void SendGetRequestSync(std::string url, std::string &content, std::string &error);

private:

	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
};

} // namespace http
} // namespace postchain
} // namespace chromia
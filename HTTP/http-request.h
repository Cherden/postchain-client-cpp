/**
* cpp - http wrapper.
* this header is UE4 independent.
* TO-DO replace implementation with pure openssl lib, to make it UE4 full independent
*/

#include <string>
#include <functional>

using namespace std;

namespace chromia {
namespace postchain {
namespace http {

class HttpRequest
{
public:
	HttpRequest(std::string url, std::function<void(int, std::string)> callback);

	void SetContent(std::string content);

	void Get();

	void Post();

private:

	std::string url_;
	std::string content_;
	std::function<void(int, std::string)> callback_;

	///*Called when the server has responded to http request*/
	//void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

} // namespace http
} // namespace postchain
} // namespace chromia
//// put this at the top of your .h file above #includes
//// UE4: allow Windows platform types to avoid naming collisions
//// must be undone at the bottom of this file!
//#include "Windows/AllowWindowsPlatformTypes.h"
//#include "Windows/prewindowsapi.h"
//
//#include "http-req-client.h"
//#include <string>
//#include <curl/curl.h>
//#include "CoreMinimal.h"
//#include "../../ChromaUnreal/Utils.h"
//
//static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
//{
//	((std::string*)userp)->append((char*)contents, size * nmemb);
//	return size * nmemb;
//}
//
//void PostchainHttpRequest::GetSync()
//{
//	//CURL *curl;
//	//CURLcode res;
//	//std::string readBuffer;
//
//	//curl = curl_easy_init();
//	//if (curl) {
//	//	curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/success");
//	//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//	//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
//	//	res = curl_easy_perform(curl);
//	//	curl_easy_cleanup(curl);
//	//	//std::cout << readBuffer << std::endl;
//	//}
//	//UE_LOG(LogTemp, Warning, TEXT("Result: %s"), *ChromaUtils::STDStringToFString(readBuffer));
//}
//
//// put this at the bottom of the .h file
//// UE4: disallow windows platform types
//// this was enabled at the top of the file
//#include "Windows/PostWindowsApi.h"
//#include "Windows/HideWindowsPlatformTypes.h"
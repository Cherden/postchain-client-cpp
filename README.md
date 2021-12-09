# postchain-client-cpp
C++ translation from https://bitbucket.org/chromawallet/postchain-client

### How to build on windows

1) Install OpenSSH c++ lib (https://github.com/openssh)
2) Install CURL c++ lib (https://github.com/curl/curl)
3) Clone repo in clean dir, open terminal and run (Replace paths with your own):
4) `cmake -DCURL_LIB="Curl/Root/build/lib/x64/libcurl.lib" -DCURL_INCLUDE_DIR="Curl/Root/include" -G "Visual Studio 15 2017" -A x64 ..`
5) Open the new generated *.sln from /build
6) Select X64 build
7) If you need to compile tests, add a global macro definition `#define COMPILE_TESTS`
7) Build and Run
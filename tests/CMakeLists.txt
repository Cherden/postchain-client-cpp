include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

FetchContent_MakeAvailable(googletest)


add_executable(
    postchain_client_test
    TestUtil/test_util.cpp
    ASN1/writer_test.cpp
    ASN1/reader_test.cpp
    GTV/value_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../src/PostchainClient/ASN1/writer.h
    ${CMAKE_CURRENT_LIST_DIR}/../src/PostchainClient/ASN1/reader.h
)

target_link_libraries(
    postchain_client_test
    postchain_client
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(postchain_client_test)
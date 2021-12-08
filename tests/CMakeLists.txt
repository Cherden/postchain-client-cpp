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

    FT3/account_test.cpp
    FT3/asset_balance_test.cpp
    FT3/asset_test.cpp
    FT3/auth_descriptor_rule_test.cpp
    FT3/blockchain_test.cpp
    FT3/rate_limit_test.cpp
    FT3/transfer_test.cpp
    FT3/ft3_tests_main.cpp

    SSO/sso_test.cpp

    TestUtil/blockchain_util.cpp
    TestUtil/account_builder.cpp
    TestUtil/directory_service_util.cpp
    TestUtil/fake_directory_service_util.cpp
    TestUtil/test_user.cpp
    TestUtil/test_util.cpp

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
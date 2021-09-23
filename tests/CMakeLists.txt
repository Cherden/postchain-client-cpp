option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

if(ENABLE_UNIT_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
    )

    FetchContent_MakeAvailable(googletest)

    add_executable(
        postchain_client_test
        ASN1/writer_test.cc
        ASN1/reader_test.cc
        ${CMAKE_CURRENT_LIST_DIR}/../src/ASN1/writer.h
        ${CMAKE_CURRENT_LIST_DIR}/../src/ASN1/reader.h
    )

    target_link_libraries(
        postchain_client_test
        postchain_client
        gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(postchain_client_test)
endif()
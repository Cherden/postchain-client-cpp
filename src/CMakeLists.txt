add_library(
    postchain_client
    STATIC
    PostchainClient/ASN1/writer.cpp
    PostchainClient/ASN1/reader.cpp

    PostchainClient/GTV/abstract_value.cpp
    PostchainClient/GTV/Merkle/Proof/merkle_proof.cpp

    PostchainClient/GTX/dict_pair.cpp
    PostchainClient/GTX/gtx.cpp
    PostchainClient/GTX/gtx_value.cpp

    PostchainClient/blockchain_client.cpp
    PostchainClient/operation.cpp
    PostchainClient/postchain_transaction.cpp
    PostchainClient/postchain_util.cpp
    PostchainClient/SECP256K/src/secp256k1.c

    HTTP/httprequest.cpp

    FT3/Core/Blockchain/blockchain_info.cpp
    FT3/Core/Blockchain/blockchain_session.cpp
    FT3/Core/Blockchain/blockchain.cpp
    FT3/Core/Blockchain/directory_service_base.cpp
    FT3/Core/key_pair.cpp
    FT3/Core/operation.cpp
    FT3/Core/transaction_builder.cpp
    FT3/Core/transaction.cpp
    FT3/Core/util.cpp

    FT3/User/AuthDescriptor/auth_descriptor_factory.cpp
    FT3/User/AuthDescriptor/auth_descriptor_rule.cpp
    FT3/User/AuthDescriptor/multi_signature_auth_descriptor.cpp
    FT3/User/AuthDescriptor/single_signature_auth_descriptor.cpp

    FT3/User/account_dev_operations.cpp
    FT3/User/account_operations.cpp
    FT3/User/asset_balance.cpp
    FT3/User/account.cpp
    FT3/User/asset.cpp
    FT3/User/rate_limit.cpp
    FT3/User/transfer.cpp
    FT3/User/user.cpp
)

# file(GLOB TEMP_SRC
#      "PostchainClient/*.h"
#      "PostchainClient/*.cpp"
# )

# add_library(postchain_client SHARED ${TEMP_SRC})

# file(GLOB APP_SOURCES */*.cpp)
# foreach (testsourcefile ${APP_SOURCES})
#     get_filename_component(testname ${testsourcefile} NAME_WLE)
#     get_filename_component(dirname ${testsourcefile} DIRECTORY)
#     file(GLOB dir_src CONFIGURE_DEPENDS
#             "${dirname}/*.h"
#             "${dirname}/*.cpp"
#             )
#     message("${testname}.cpp | ${dir_src}")
#     add_library("${testname}.cpp" ${dir_src})
# endforeach (testsourcefile ${APP_SOURCES})


find_package(OpenSSL REQUIRED)
target_link_libraries(postchain_client OpenSSL::SSL)

# ==================== Link curl ===================
target_include_directories(postchain_client
    PRIVATE
    ${CURL_INCLUDE_DIR}
)
target_link_libraries(postchain_client ${CURL_LIB})
# ==================================================

target_compile_definitions(postchain_client PUBLIC postchain_client)

# set_target_properties(postchain_client PROPERTIES VERSION ${PROJECT_VERSION})
# set_target_properties(postchain_client PROPERTIES PUBLIC_HEADER include/mylib.h)

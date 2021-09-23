add_library(
    postchain_client
    SHARED
    ASN1/writer.cc
    ASN1/reader.cc
    postchain_util.cc
)

target_compile_definitions(postchain_client PUBLIC postchain_client)

# set_target_properties(postchain_client PROPERTIES VERSION ${PROJECT_VERSION})
# set_target_properties(postchain_client PROPERTIES PUBLIC_HEADER include/mylib.h)
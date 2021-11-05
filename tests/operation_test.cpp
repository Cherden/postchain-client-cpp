#ifdef POSTCHAIN_TESTS

#include "../src/operation.h"

#include <gtest/gtest.h>

#include <string>

#include "../src/postchain_util.h"

using namespace chromia::postchain;

TEST(Operation, EmptyOperationTest) {
    Operation op("test");
    auto encoded_op = op.ToAbstractValue()->Encode();

    auto expected = "a50a3008a2060c0474657374";
    EXPECT_EQ(expected, PostchainUtil::ByteVectorToHexString(encoded_op));
}

TEST(Operation, SimpleOperationTest) {
    Operation op("test");
    op << "teststring";
    auto encoded_op = op.ToAbstractValue()->Encode();

    auto expected = "a5183016a2060c0474657374a20c0c0a74657374737472696e67";
    EXPECT_EQ(expected, PostchainUtil::ByteVectorToHexString(encoded_op));
}

TEST(Operation, ArrayOperationTest) {
    Operation op("test");
    op << "teststring" << std::vector<std::string>{"hello", "world"};
    auto encoded_op = op.ToAbstractValue()->Encode();

    auto expected =
        "a52e302ca2060c0474657374a20c0c0a74657374737472696e67a5143012a2070c0568"
        "656c6c6fa2070c05776f726c64";
    EXPECT_EQ(expected, PostchainUtil::ByteVectorToHexString(encoded_op));
}

TEST(Operation, FullOperationTest) {
    Operation op("test");
    op << "teststring" << 123 << std::vector<unsigned char>{0xaf, 0xfe} << true;
    auto encoded_op = op.ToAbstractValue()->Encode();

    auto expected =
        "a5283026a2060c0474657374a20c0c0a74657374737472696e67a30302017ba1040402"
        "affea303020101";
    EXPECT_EQ(expected, PostchainUtil::ByteVectorToHexString(encoded_op));
}

#endif // POSTCHAIN_TESTS
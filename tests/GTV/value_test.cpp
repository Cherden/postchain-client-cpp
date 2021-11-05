#ifdef POSTCHAIN_TESTS
#include <gtest/gtest.h>

#include <limits>

#include "../../src/GTV/abstract_value.h"
#include "../../src/GTV/abstract_value_facotry.h"
#include "../../src/GTV/array_value.h"
#include "../../src/GTV/dict_value.h"
#include "../../src/GTV/integer_value.h"
#include "../../src/GTV/null_value.h"
#include "../../src/GTV/octet_string_value.h"
#include "../../src/GTV/utf8_string_value.h"
#include "../../src/postchain_util.h"

using namespace chromia::postchain;

TEST(GTV, NullTest) {
    gtv::NullValue value;

    std::string expected_hex = "a0020500";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, SimpleUTF8StingTest) {
    gtv::UTF8StringValue value("test");

    std::string expected_hex = "a2060c0474657374";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, EmptyUTF8StingTest) {
    gtv::UTF8StringValue value("");

    std::string expected_hex = "a2020c00";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, LongUTF8StingTest) {
    gtv::UTF8StringValue value(std::string(2048, 'x'));

    std::string expected_hex =
        "a28208040c820800787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878787878787878787878"
        "7878787878787878787878787878787878787878787878787878";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, SimpleIntegerTest) {
    gtv::IntegerValue value(1337);

    std::string expected_hex = "a30402020539";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, NegativeIntegerTest) {
    gtv::IntegerValue value(-1337);

    std::string expected_hex = "a3040202fac7";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, MaxIntegerTest) {
    gtv::IntegerValue value(std::numeric_limits<long long>::max());

    std::string expected_hex = "a30a02087fffffffffffffff";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, MinIntegerTest) {
    gtv::IntegerValue value(std::numeric_limits<long long>::min());

    std::string expected_hex = "a30a02088000000000000000";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, SimpleOctetStringTest) {
    gtv::OctetStringValue value({0xaf, 0xfe, 0xca, 0xfe});

    std::string expected_hex = "a1060404affecafe";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, EmptyOctetStringTest) {
    gtv::OctetStringValue value({});

    std::string expected_hex = "a1020400";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, SimpleArrayTest) {
    gtv::ArrayValue value;
    value.Add(std::make_shared<gtv::UTF8StringValue>("test"));

    std::string expected_hex = "a50a3008a2060c0474657374";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, EmptyArrayTest) {
    gtv::ArrayValue value;

    std::string expected_hex = "a5023000";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, ArrayInArrayTest) {
    gtv::ArrayValue value;

    auto inner_value = std::make_shared<gtv::ArrayValue>();
    inner_value->Add(std::make_shared<gtv::IntegerValue>(
        std::numeric_limits<long long>::min()));

    value.Add(inner_value);

    std::string expected_hex = "a5123010a50e300ca30a02088000000000000000";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, FullArrayTest) {
    gtv::ArrayValue value;
    value.Add(std::make_shared<gtv::UTF8StringValue>("test"));
    value.Add(std::make_shared<gtv::NullValue>());
    value.Add(std::make_shared<gtv::IntegerValue>(
        std::numeric_limits<long long>::max()));

    std::vector<unsigned char> byte_array{0xde, 0xad, 0xbe, 0xef};
    value.Add(std::make_shared<gtv::OctetStringValue>(byte_array));
    value.Add(std::make_shared<gtv::ArrayValue>());

    std::string expected_hex =
        "a5263024a2060c0474657374a0020500a30a02087fffffffffffffffa1060404deadbe"
        "efa5023000";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, BuildNullTest) {
    auto value = gtv::AbstractValueFactory::Build(nullptr);
    gtv::NullValue expected_value;

    EXPECT_EQ(expected_value.Encode(), value->Encode());
}

TEST(GTV, BuildIntegerTest) {
    auto value = gtv::AbstractValueFactory::Build(1337);
    gtv::IntegerValue expected_value(1337);

    EXPECT_EQ(expected_value.Encode(), value->Encode());
}

TEST(GTV, BuildLargeIntegerTest) {
    long long number = std::numeric_limits<long long>::max();
    auto value = gtv::AbstractValueFactory::Build(number);
    gtv::IntegerValue expected_value(number);

    EXPECT_EQ(expected_value.Encode(), value->Encode());
}

TEST(GTV, BuildNegativeIntegerTest) {
    long long number = std::numeric_limits<long long>::min();
    auto value = gtv::AbstractValueFactory::Build(number);
    gtv::IntegerValue expected_value(number);

    EXPECT_EQ(expected_value.Encode(), value->Encode());
}

TEST(GTV, BuildUTF8StringTest) {
    auto value = gtv::AbstractValueFactory::Build(std::string("test"));
    gtv::UTF8StringValue expected_value("test");

    EXPECT_EQ(expected_value.Encode(), value->Encode());
}

TEST(GTV, BuildUTF8StringCharArrayTest) {
    auto value = gtv::AbstractValueFactory::Build("test_char");
    gtv::UTF8StringValue expected_value("test_char");

    EXPECT_EQ(expected_value.Encode(), value->Encode());
}

TEST(GTV, BuildBooleanTest) {
    auto value = gtv::AbstractValueFactory::Build(true);
    gtv::IntegerValue expected_value(1);

    EXPECT_EQ(expected_value.Encode(), value->Encode());
}

TEST(GTV, BuildOctetStringTest) {
    std::vector<unsigned char> bytes{0xaf, 0xfe};
    auto value = gtv::AbstractValueFactory::Build(bytes);
    gtv::OctetStringValue expected_value(bytes);

    EXPECT_EQ(expected_value.Encode(), value->Encode());
}

TEST(GTV, BuildArrayTest) {
    auto value = gtv::AbstractValueFactory::EmptyArray();
    gtv::ArrayValue expected_value;
    auto inner_value = gtv::AbstractValueFactory::Build(true);

    value->Add(inner_value);
    expected_value.Add(inner_value);

    EXPECT_EQ(expected_value.Encode(), value->Encode());
}

TEST(GTV, BuildArrayInitializeTest) {
    std::vector<std::shared_ptr<gtv::AbstractValue>> inner_values{
        gtv::AbstractValueFactory::Build(true),
        gtv::AbstractValueFactory::Build("test"),
        gtv::AbstractValueFactory::Build(1337),
        gtv::AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector(
            "93e262166db3ae72872a28df9a8fbf9f411eb83040088c70c0953f6724f924a"
            "0")),
    };

    auto value = gtv::AbstractValueFactory::Build(inner_values);
    gtv::ArrayValue expected_value(inner_values);

    EXPECT_EQ(expected_value.Encode(), value->Encode());
}

TEST(GTV, SimpleHashTest) {
    auto value = gtv::AbstractValueFactory::Build(std::string("test"));

    auto hash = gtv::AbstractValue::Hash(value);
    std::string expected =
        "bd0582e368dfb006fa34a75f372f761d3cfb6cd58bf5e4853addf767f55d8265";
    EXPECT_EQ(expected, PostchainUtil::ByteVectorToHexString(hash));
}

TEST(GTV, ArrayHashTest) {
    gtv::ArrayValue value;
    value.Add(std::make_shared<gtv::UTF8StringValue>("test"));
    value.Add(std::make_shared<gtv::NullValue>());

    auto hash =
        gtv::AbstractValue::Hash(std::make_shared<gtv::ArrayValue>(value));
    std::string expected =
        "aca6ea1208967ebc2d140d1496dde2d09e40adbd843161db9d237c5504e9e0eb";
    EXPECT_EQ(expected, PostchainUtil::ByteVectorToHexString(hash));
}

TEST(GTV, FullArrayHashTest) {
    std::vector<std::shared_ptr<gtv::AbstractValue>> inner_values{
        gtv::AbstractValueFactory::Build("test"),
        gtv::AbstractValueFactory::Build(nullptr),
        gtv::AbstractValueFactory::Build(std::numeric_limits<long long>::max()),
        gtv::AbstractValueFactory::Build(
            std::vector<unsigned char>{0xde, 0xad, 0xbe, 0xef}),
        gtv::AbstractValueFactory::EmptyArray()};
    auto value = gtv::AbstractValueFactory::Build(inner_values);

    auto hash = gtv::AbstractValue::Hash(value);
    std::string expected =
        "e74615c8e242ee865655b24a17b1454e0f14523520384903682cd31500907a2d";
    EXPECT_EQ(expected, PostchainUtil::ByteVectorToHexString(hash));
}

#endif // POSTCHAIN_TESTS
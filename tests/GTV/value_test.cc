#include <gtest/gtest.h>

#include <limits>

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

    std::string expected_hex = "A0020500";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, SimpleUTF8StingTest) {
    gtv::UTF8StringValue value("test");

    std::string expected_hex = "A2060C0474657374";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, EmptyUTF8StingTest) {
    gtv::UTF8StringValue value("");

    std::string expected_hex = "A2020C00";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, LongUTF8StingTest) {
    gtv::UTF8StringValue value(std::string(2048, 'x'));

    std::string expected_hex =
        "A28208040C820800787878787878787878787878787878787878787878787878787878"
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

    std::string expected_hex = "A30402020539";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, NegativeIntegerTest) {
    gtv::IntegerValue value(-1337);

    std::string expected_hex = "A3040202FAC7";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, MaxIntegerTest) {
    gtv::IntegerValue value(std::numeric_limits<long long>::max());

    std::string expected_hex = "A30A02087FFFFFFFFFFFFFFF";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, MinIntegerTest) {
    gtv::IntegerValue value(std::numeric_limits<long long>::min());

    std::string expected_hex = "A30A02088000000000000000";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, SimpleOctetStringTest) {
    gtv::OctetStringValue value({0xaf, 0xfe, 0xca, 0xfe});

    std::string expected_hex = "A1060404AFFECAFE";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, EmptyOctetStringTest) {
    gtv::OctetStringValue value({});

    std::string expected_hex = "A1020400";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, SimpleArrayTest) {
    gtv::ArrayValue value;
    value.Add(std::make_shared<gtv::UTF8StringValue>("test"));

    std::string expected_hex = "A50A3008A2060C0474657374";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, EmptyArrayTest) {
    gtv::ArrayValue value;

    std::string expected_hex = "A5023000";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(value.Encode()));
}

TEST(GTV, ArrayInArrayTest) {
    gtv::ArrayValue value;

    auto inner_value = std::make_shared<gtv::ArrayValue>();
    inner_value->Add(std::make_shared<gtv::IntegerValue>(
        std::numeric_limits<long long>::min()));

    value.Add(inner_value);

    std::string expected_hex = "A5123010A50E300CA30A02088000000000000000";
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
        "A5263024A2060C0474657374A0020500A30A02087FFFFFFFFFFFFFFFA1060404DEADBE"
        "EFA5023000";
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
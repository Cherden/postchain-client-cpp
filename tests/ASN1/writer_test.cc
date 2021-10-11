#include "../../src/ASN1/writer.h"

#include <gtest/gtest.h>

#include <limits>

#include "../../src/postchain_util.h"

using namespace chromia::postchain;

TEST(ASN1Writer, NullTest) {
    asn1::Writer writer;
    writer.WriteNull();

    std::string expected_hex = "0500";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, OctetStringTest) {
    asn1::Writer writer;
    writer.WriteOctetString({0xaf, 0xfe});

    std::string expected_hex = "0402affe";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, EmptyOctetStringTest) {
    asn1::Writer writer;
    writer.WriteOctetString({});

    std::string expected_hex = "0400";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, BRIDOctetStringTest) {
    asn1::Writer writer;
    auto brid = PostchainUtil::HexStringToByteVector(
        "E2BE5C617CE50AFD0882A753C6FDA9C4D925EEDAC50DB97E33F457826A856DE0");
    writer.WriteOctetString({brid.begin(), brid.end()});

    std::string expected_hex =
        "0420e2be5c617ce50afd0882a753c6fda9c4d925eedac50db97e33f457826a856de0";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, UTF8StringTest) {
    asn1::Writer writer;
    writer.WriteUTF8String("Hello World!");

    std::string expected_hex = "0c0c48656c6c6f20576f726c6421";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, EmptyUTF8StringTest) {
    asn1::Writer writer;
    writer.WriteUTF8String("");

    std::string expected_hex = "0c00";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, SpecialCharactersUTF8StringTest) {
    asn1::Writer writer;
    writer.WriteUTF8String("Swedish: Åå Ää Öö");
    writer.WriteUTF8String("Danish/Norway: Ææ Øø Åå");
    writer.WriteUTF8String("German/Finish: Ää Öö Üü");
    writer.WriteUTF8String("Greek lower: αβγδϵζηθικλμνξοπρστυϕχψω");
    writer.WriteUTF8String("Greek upper: ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ");
    writer.WriteUTF8String(
        "Russian: "
        "АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя");

    std::string expected_hex =
        "0c17537765646973683a20c385c3a520c384c3a420c396c3b60c1d44616e6973682f4e"
        "6f727761793a20c386c3a620c398c3b820c385c3a50c1d4765726d616e2f46696e6973"
        "683a20c384c3a420c396c3b620c39cc3bc0c3d477265656b206c6f7765723a20ceb1ce"
        "b2ceb3ceb4cfb5ceb6ceb7ceb8ceb9cebacebbcebccebdcebecebfcf80cf81cf83cf84"
        "cf85cf95cf87cf88cf890c3d477265656b2075707065723a20ce91ce92ce93ce94ce95"
        "ce96ce97ce98ce99ce9ace9bce9cce9dce9ece9fcea0cea1cea3cea4cea5cea6cea7ce"
        "a8cea90c81895275737369616e3a20d090d0b0d091d0b1d092d0b2d093d0b3d094d0b4"
        "d095d0b5d081d191d096d0b6d097d0b7d098d0b8d099d0b9d09ad0bad09bd0bbd09cd0"
        "bcd09dd0bdd09ed0bed09fd0bfd0a1d181d0a2d182d0a3d183d0a4d184d0a5d185d0a6"
        "d186d0a7d187d0a8d188d0a9d189d0aad18ad0abd18bd0acd18cd0add18dd0aed18ed0"
        "afd18f";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, IntegerTest) {
    asn1::Writer writer;
    writer.WriteInteger(42424242);

    std::string expected_hex = "0204028757b2";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, SepcialIntegerTest) {
    asn1::Writer writer;
    writer.WriteInteger(256);

    std::string expected_hex = "02020100";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, NegativeSepcialIntegerTest) {
    asn1::Writer writer;
    writer.WriteInteger(-256);

    std::string expected_hex = "0202ff00";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, EdgeIntegerTest) {
    asn1::Writer writer;
    writer.WriteInteger(-1);

    std::string expected_hex = "0201ff";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, NegativeIntegerTest) {
    asn1::Writer writer;
    writer.WriteInteger(-42424242);

    std::string expected_hex = "0204fd78a84e";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, LimitIntegerTest) {
    asn1::Writer writer;
    writer.WriteInteger(std::numeric_limits<long long>::min());
    writer.WriteInteger(std::numeric_limits<long long>::max());

    std::string expected_hex = "0208800000000000000002087fffffffffffffff";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, SequenceTest) {
    asn1::Writer writer;
    writer.PushSequence();

    writer.PushSequence();
    auto brid = PostchainUtil::HexStringToByteVector(
        "E2BE5C617CE50AFD0882A753C6FDA9C4D925EEDAC50DB97E33F457826A856DE0");
    writer.WriteOctetString(brid);
    writer.PopSequence();

    writer.PushSequence();
    writer.PushSequence();
    writer.WriteUTF8String("test_op1");
    writer.WriteUTF8String("arg1");
    writer.WriteInteger(42);
    writer.PopSequence();
    writer.PushSequence();
    writer.WriteUTF8String("test_op2");
    writer.PopSequence();
    writer.PopSequence();

    writer.PopSequence();

    std::string expected_hex =
        "304730220420e2be5c617ce50afd0882a753c6fda9c4d925eedac50db97e33f457826a"
        "856de0302130130c08746573745f6f70310c046172673102012a300a0c08746573745f"
        "6f7032";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

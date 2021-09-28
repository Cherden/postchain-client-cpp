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

    std::string expected_hex = "0402AFFE";
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
        "0420E2BE5C617CE50AFD0882A753C6FDA9C4D925EEDAC50DB97E33F457826A856DE0";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, UTF8StringTest) {
    asn1::Writer writer;
    writer.WriteUTF8String("Hello World!");

    std::string expected_hex = "0C0C48656C6C6F20576F726C6421";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, EmptyUTF8StringTest) {
    asn1::Writer writer;
    writer.WriteUTF8String("");

    std::string expected_hex = "0C00";
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
        "0C17537765646973683A20C385C3A520C384C3A420C396C3B60C1D44616E6973682F4E"
        "6F727761793A20C386C3A620C398C3B820C385C3A50C1D4765726D616E2F46696E6973"
        "683A20C384C3A420C396C3B620C39CC3BC0C3D477265656B206C6F7765723A20CEB1CE"
        "B2CEB3CEB4CFB5CEB6CEB7CEB8CEB9CEBACEBBCEBCCEBDCEBECEBFCF80CF81CF83CF84"
        "CF85CF95CF87CF88CF890C3D477265656B2075707065723A20CE91CE92CE93CE94CE95"
        "CE96CE97CE98CE99CE9ACE9BCE9CCE9DCE9ECE9FCEA0CEA1CEA3CEA4CEA5CEA6CEA7CE"
        "A8CEA90C81895275737369616E3A20D090D0B0D091D0B1D092D0B2D093D0B3D094D0B4"
        "D095D0B5D081D191D096D0B6D097D0B7D098D0B8D099D0B9D09AD0BAD09BD0BBD09CD0"
        "BCD09DD0BDD09ED0BED09FD0BFD0A1D181D0A2D182D0A3D183D0A4D184D0A5D185D0A6"
        "D186D0A7D187D0A8D188D0A9D189D0AAD18AD0ABD18BD0ACD18CD0ADD18DD0AED18ED0"
        "AFD18F";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, IntegerTest) {
    asn1::Writer writer;
    writer.WriteInteger(42424242);

    std::string expected_hex = "0204028757B2";
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

    std::string expected_hex = "0202FF00";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, EdgeIntegerTest) {
    asn1::Writer writer;
    writer.WriteInteger(-1);

    std::string expected_hex = "0201FF";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, NegativeIntegerTest) {
    asn1::Writer writer;
    writer.WriteInteger(-42424242);

    std::string expected_hex = "0204FD78A84E";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

TEST(ASN1Writer, LimitIntegerTest) {
    asn1::Writer writer;
    writer.WriteInteger(std::numeric_limits<long long>::min());
    writer.WriteInteger(std::numeric_limits<long long>::max());

    std::string expected_hex = "0208800000000000000002087FFFFFFFFFFFFFFF";
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
        "304730220420E2BE5C617CE50AFD0882A753C6FDA9C4D925EEDAC50DB97E33F457826A"
        "856DE0302130130C08746573745F6F70310C046172673102012A300A0C08746573745F"
        "6F7032";
    EXPECT_EQ(expected_hex,
              PostchainUtil::ByteVectorToHexString(writer.Encode()));
}

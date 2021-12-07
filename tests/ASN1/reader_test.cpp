//#include "../../src/PostchainClient/ASN1/reader.h"
//#include <gtest/gtest.h>
//#include <limits>
//#include "../../src/PostchainClient/ASN1/writer.h"
//#include "../../src/PostchainClient/postchain_util.h"
//
//using namespace chromia::postchain;
//
//TEST(ASN1Reader, NullTest) {
//    asn1::Writer writer;
//    writer.WriteNull();
//
//    asn1::Reader reader(writer.Encode());
//    reader.ReadNull();
//}
//
//TEST(ASN1Reader, OctetStringTest) {
//    asn1::Writer writer;
//    std::vector<unsigned char> bytes{0xaf, 0xfe};
//    writer.WriteOctetString(bytes);
//
//    asn1::Reader reader(writer.Encode());
//    EXPECT_EQ(bytes, reader.ReadOctetString());
//}
//
//TEST(ASN1Reader, EmptyOctetStringTest) {
//    asn1::Writer writer;
//    std::vector<unsigned char> bytes{};
//    writer.WriteOctetString(bytes);
//
//    asn1::Reader reader(writer.Encode());
//    EXPECT_EQ(bytes, reader.ReadOctetString());
//}
//
//TEST(ASN1Reader, BRIDOctetStringTest) {
//    asn1::Writer writer;
//    auto brid = PostchainUtil::HexStringToByteVector(
//        "E2BE5C617CE50AFD0882A753C6FDA9C4D925EEDAC50DB97E33F457826A856DE0");
//    std::vector<unsigned char> bytes{brid.begin(), brid.end()};
//    writer.WriteOctetString(bytes);
//
//    asn1::Reader reader(writer.Encode());
//    EXPECT_EQ(bytes, reader.ReadOctetString());
//}
//
//TEST(ASN1Reader, UTF8StringTest) {
//    asn1::Writer writer;
//    std::string str = "Hello World!";
//    writer.WriteUTF8String(str);
//
//    asn1::Reader reader(writer.Encode());
//    EXPECT_EQ(str, reader.ReadUTF8String());
//}
//
//TEST(ASN1Reader, EmptyUTF8StringTest) {
//    asn1::Writer writer;
//    std::string str = "";
//    writer.WriteUTF8String(str);
//
//    asn1::Reader reader(writer.Encode());
//    EXPECT_EQ(str, reader.ReadUTF8String());
//}
//
//TEST(ASN1Reader, SpecialCharactersUTF8StringTest) {
//    asn1::Writer writer;
//    std::string strings[]{
//        "Swedish: Åå Ää Öö",
//        "Danish/Norway: Ææ Øø Åå",
//        "German/Finish: Ää Öö Üü",
//        "Greek lower: αβγδϵζηθικλμνξοπρστυϕχψω",
//        "Greek upper: ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ",
//        "Russian: "
//        "АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя"};
//
//    for (auto str : strings) {
//        writer.WriteUTF8String(str);
//    }
//
//    asn1::Reader reader(writer.Encode());
//    for (auto str : strings) {
//        EXPECT_EQ(str, reader.ReadUTF8String());
//    }
//}
//
//TEST(ASN1Reader, IntegerTest) {
//    asn1::Writer writer;
//    long long numbers[]{std::numeric_limits<long long>::min(),
//                        -42424242,
//                        -256,
//                        -1,
//                        0,
//                        1,
//                        256,
//                        42424242,
//                        std::numeric_limits<long long>::max()};
//
//    for (auto number : numbers) {
//        writer.WriteInteger(number);
//    }
//
//    asn1::Reader reader(writer.Encode());
//    for (auto number : numbers) {
//        EXPECT_EQ(number, reader.ReadInteger());
//    }
//}
//
//TEST(ASN1Reader, SequenceTest) {
//    asn1::Writer writer;
//    writer.PushSequence();
//
//    writer.PushSequence();
//    auto brid = PostchainUtil::HexStringToByteVector(
//        "E2BE5C617CE50AFD0882A753C6FDA9C4D925EEDAC50DB97E33F457826A856DE0");
//    writer.WriteOctetString(brid);
//    writer.PopSequence();
//
//    writer.PushSequence();
//    writer.PushSequence();
//    std::string op1_name = "test_op1";
//    std::string op1_arg1 = "arg1";
//    int op1_arg2 = 42;
//    writer.WriteUTF8String(op1_name);
//    writer.WriteUTF8String(op1_arg1);
//    writer.WriteInteger(op1_arg2);
//    writer.PopSequence();
//    writer.PushSequence();
//    std::string op2_name = "test_op2";
//    writer.WriteUTF8String(op2_name);
//    writer.PopSequence();
//    writer.PopSequence();
//
//    writer.PopSequence();
//
//    asn1::Reader reader(writer.Encode());
//    auto main_seq = reader.ReadSequence();
//    auto brid_seq = main_seq.ReadSequence();
//    EXPECT_EQ(brid, brid_seq.ReadOctetString());
//
//    auto op_seq = main_seq.ReadSequence();
//    auto op1_seq = op_seq.ReadSequence();
//    EXPECT_EQ(op1_name, op1_seq.ReadUTF8String());
//    EXPECT_EQ(op1_arg1, op1_seq.ReadUTF8String());
//    EXPECT_EQ(op1_arg2, op1_seq.ReadInteger());
//
//    auto op2_seq = op_seq.ReadSequence();
//    EXPECT_EQ(op2_name, op2_seq.ReadUTF8String());
//}
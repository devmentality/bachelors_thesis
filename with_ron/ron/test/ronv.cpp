//
// Created by gritzko on 7/26/20.
//
#include "../ron.hpp"
#include "test.hpp"

using namespace std;
using namespace ron;

TEST(RONv, Atoms) {
    Float f{3.1415};
    Integer i{INT64_MAX};
    auto n = Uuid::Name(TEST);
    VarIntStream s{};
    s.Create();
    ASSERT_OK(s.FeedWordFlipped((Word&)f));
    ASSERT_OK(s.FeedWord((Word&)i));
    ASSERT_OK(s.FeedWordFlipped(n.first));
    ASSERT_OK(s.FeedWordFlipped(n.second));
    auto data = s.Filled();
    Float f2;
    Integer i2;
    auto n2 = Uuid::Name(0);
    VarIntStream r{};
    r.Create();
    r.FeedSpan(data);
    ASSERT_OK(r.DrainFlippedWord((Word&)f2));
    ASSERT_OK(r.DrainWord((Word&)i2));
    ASSERT_OK(r.DrainFlippedWord(n2.first));
    ASSERT_OK(r.DrainFlippedWord(n2.second));
    ASSERT_EQ(f, f2);
    ASSERT_EQ(i, i2);
    ASSERT_EQ(n, n2);
    uint8_t b;
    ASSERT_EQ(NODATA, r.DrainByte(b));
}

TEST(RONv, Cycle) {
    Tuple v{};
    const char* str = "strinг";
    v.PushFloat(3.1415);
    v.PushInteger(8);
    v.PushUuid(Uuid::Lamport(12, 34));
    v.PushInteger(-1);
    v.PushUtf8String(str);
    RONvStream s{};
    s.Create();
    ASSERT_OK(s.FeedTuple(v));
    RONvStream r{};
    r.Create();
    ASSERT_OK(r.FeedSpan(s.Filled()));
    Tuple v2{};
    ASSERT_OK(r.DrainTuple(v2));
    uint32_t u32;
    ASSERT_EQ(r.DrainIndex(u32), NODATA);
    Tuple::Box box;
    Tuple::BoxReader reader{v2};

    ASSERT_OK(reader.ReadBox(box));
    ASSERT_EQ(box.Type(), FLOAT);

    ASSERT_OK(reader.ReadBox(box));
    ASSERT_EQ(box.Type(), INT);

    ASSERT_OK(reader.ReadBox(box));
    ASSERT_EQ(box.Type(), ID);

    ASSERT_OK(reader.ReadBox(box));
    ASSERT_EQ(box.Type(), INT);

    ASSERT_OK(reader.ReadBox(box));
    ASSERT_EQ(box.Type(), STRING);

    ASSERT_TRUE(reader.IsEmpty());
    ASSERT_EQ(reader.ReadBox(box), ENDOFRAME);
}


TEST(RONv, Pipe) {
    RONvStream reader{}, writer{};
    reader.Open(Stream::CLOSED, Stream::TMP);
    writer.Open(Stream::CLOSED, Stream::TMP);
    auto one = Uuid::Yolo(1, 1, TEST);
    auto two = one.Inc();
    auto three = two.Inc();
    auto five = Uuid::Yolo(5, 5, TEST);
    auto seven = Uuid::Yolo(7, 7, TEST);
    Op op1{two, one, Integer{3}};
    Op op2{three, two, YOLO_ZERO};
    Op op5{five, three, YOLO_ZERO};
    Op op7{seven, one, "a", "b"};
    Op copy{};

    ASSERT_OK(writer.FeedOp(op1));
    ASSERT_OK(writer.FeedOp(op2));
    ASSERT_OK(writer.FeedOp(op5));
    ASSERT_OK(writer.FeedOp(op7));
    ASSERT_OK(writer.FeedEOF());

    ASSERT_OK(reader.FeedSpan(writer.Filled()));

    ASSERT_OK(reader.DrainOp(copy));
    ASSERT_TRUE(op1.IsEqual(copy));
    ASSERT_OK(reader.DrainOp(copy));
    ASSERT_TRUE(op2.IsEqual(copy));
    ASSERT_OK(reader.DrainOp(copy));
    ASSERT_TRUE(op5.IsEqual(copy));
    ASSERT_OK(reader.DrainOp(copy));
    ASSERT_TRUE(op7.IsEqual(copy));
}

TEST(RONv, Million) {
    TmpDir tmp{"RONvMillion"};
    RONvStream writer{};
    ASSERT_OK(writer.Create("a.ronv", Stream::WRITE));
    auto one = Uuid::Yolo(1, 1, TEST);
    auto zero = Uuid::Yolo(0, 0, TEST);
    Op op{one, zero, Integer{1}};
    for (int i = 1; i <= 1000000; ++i) {
        op.Values().Clear();
        op.Values().PushInteger(i);
        ASSERT_OK(writer.FeedOp(op));
        op.NextInEvenChain();
    }
    ASSERT_OK(writer.FeedEOF());
    ASSERT_OK(writer.DrainAll());
    ASSERT_OK(writer.Close());

    RONvStream reader{};
    ASSERT_OK(reader.Open(std::string{"a.ronv"}, Stream::READ));
    Op copy{};
    for (int i = 1; i <= 1000000; ++i) {
        auto reading = reader.DrainOp(copy);
        if (reading==NODATA) {
            ASSERT_OK(reader.Feed());
            reading = reader.DrainOp(copy);
        }
        ASSERT_OK(reading);
        ASSERT_EQ(copy.ID().HomeIndex(), i);
        ASSERT_EQ(copy.ID().YarnIndex(), i);
        ASSERT_EQ(copy.ID().Origin(), TEST);
        ASSERT_EQ(copy.Ref().Inc(), copy.ID());
        auto r = copy.Values().ReadAtoms();
        Integer j;
        ASSERT_OK(r.ReadInteger(j));
        ASSERT_EQ(j, i);
    }
}

TEST(RONv, FuzzTable) {
    vector<string> inputs{
        string{"\x09\x29\x0d\x04",4}, // 1ID+
        string{"\x01\x0e\x01\x00", 4},  //  0UUID+ 1FLOAT- (float) 0UUID-
        string{"\x0c\xff\xff\xff\xff\x00", 6},  // 1STRINGx BADUNICODE
        string{"\x11\xff\x25\x00\x00\x74\x25\x00\x00",
               9},  // @0-0-xI :~~~~w-W0000-xI;
        string{"\x11\x32\x00\x00\x7f\x04\x00\x00", 8},  // 4UUID+ x0 0x x0 0?
        "\xb5",
        string{"\x00", 1},
    };
    for (auto& input : inputs) {
        RONvStream reader01{};
        RONvStream writer12{};
        ASSERT_OK(reader01.Create());
        ASSERT_OK(reader01.FeedString(input));
        ASSERT_OK(writer12.Create());
        Ops ronc{};
        ronc.emplace_back();
        while (reader01.DrainOp(ronc.back()) == OK) {
            ASSERT_OK(writer12.FeedOp(ronc.back()));
            ronc.emplace_back();
        }
        ronc.pop_back();
        if (ronc.empty()) continue;
        writer12.FeedEOF(FACT);
        RONvStream reader23{};
        reader23.Create();
        auto i = ronc.begin();
        Op op{};
        while (writer12.FilledSize() > 0) {
            ASSERT_OK(reader23.FeedFromStream(writer12, 4));
            while (reader23.DrainOp(op) == OK) {
                ASSERT_TRUE(i < ronc.end());
                ASSERT_TRUE(op.IsEqual(*i));
                ++i;
            }
        }
        ASSERT_TRUE(i == ronc.end());
    }
}

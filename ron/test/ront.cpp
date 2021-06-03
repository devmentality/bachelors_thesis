#include <iostream>

#include "../basic-streams.hpp"
#include "../op.hpp"
#include "../ron.hpp"
#include "../uuid.hpp"
#include "test.hpp"

using namespace ron;
using namespace std;

TEST(Const, Sizes) {
    ASSERT_EQ(sizeof(span8), 16);
    ASSERT_EQ(sizeof(Word), 8);
    ASSERT_EQ(sizeof(Integer), sizeof(Word));
    ASSERT_EQ(sizeof(Uuid), sizeof(Word) << 1);
    ASSERT_EQ(sizeof(Float), sizeof(Word));
}

TEST(VI, ReadWrite) {
    VarIntStream viw{};
    viw.Open(Stream::CLOSED, Stream::TMP);
    uint64_t a = WORD_PAYLOAD_BITS, _a;
    uint32_t b = INDEX_PAYLOAD_BITS, _b;
    uint8_t c = 127, _c;
    ASSERT_OK(viw.FeedWord(a));
    ASSERT_OK(viw.FeedIndex(b));
    ASSERT_OK(viw.FeedByte(c));
    ASSERT_OK(viw.DrainWord(_a));
    ASSERT_OK(viw.DrainIndex(_b));
    ASSERT_OK(viw.DrainByte(_c));
    ASSERT_EQ(_a, a);
    ASSERT_EQ(_b, b);
    ASSERT_EQ(_c, c);
}

TEST(TextFrame, NewOp) {
    Uuid id{"1lQA32+0"};
    RONtStream ront{Stream::CLOSED, Stream::TMP};
    ront.Create();
    ront.FeedOp(Op{id, LWW_RDT});
    ront.FeedOp(Op{id.Inc(), id, Uuid{"int"}, Integer{1}});
    ++id;
    ront.FeedOp(Op{id.Inc(), id, Uuid{"float"}, 3.14159265359});
    ++id;
    ront.FeedOp(Op{id.Inc(), id, Uuid{"string"}, std::string("юникод")});
    ront.FeedEOF();
    std::string correct{
        "@1lQA32+0 :lww,\n int 1,\n float 3.14159265359,\n string "
        "'юникод';\n"};
    auto fact = ront.Filled();
    ASSERT_EQ(as_string(fact), correct);

    ASSERT_EQ(Float{1.23}, 1.23);

    string badfloat = "99999999999999999999999999.999999999999e1000;";
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    ASSERT_OK(reader.FeedString(badfloat));
    Float parsed;
    auto notok =
        reader.DrainFloat(parsed, reader.Filled().begin() + badfloat.size());
    ASSERT_FALSE(notok.Ok());
    ASSERT_TRUE(isinf(parsed));
}

TEST(TextFrame, NewOp2) {
    Uuid id{"1lQA32+0"};
    RONtStream tw{Stream::CLOSED, Stream::TMP};
    ASSERT_OK(tw.FeedOp(Op{id, LWW_RDT}));
    ASSERT_OK(tw.FeedOp(Op{id.Inc(), id, Uuid{"int"}, Integer{1}}));
    ++id;
    ASSERT_OK(tw.FeedOp(Op{id.Inc(), id, Uuid{"float"}, 3.14159265359}));
    ++id;
    ASSERT_OK(
        tw.FeedOp(Op{id.Inc(), id, Uuid{"string"}, std::string("юникод")}));
    std::string correct{
        "@1lQA32+0 :lww,\n int 1,\n float 3.14159265359,\n string "
        "'юникод';\n"};
    ASSERT_OK(tw.FeedEOF(FACT));
    ASSERT_EQ(tw.Filled(), as_span8c(correct));
}

TEST(TextFrame, basic_cycle) {
    RONtStream writer{Stream::CLOSED, Stream::TMP};
    Uuid TIME1{"1+src"};
    Uuid TIME2{"2+orig"};
    Uuid LWW{"lww"};
    Uuid KEY{"key"};
    std::string VALUE{"value"};
    writer.FeedOp(Op{TIME1, LWW});
    writer.FeedOp(Op{TIME2, TIME1, KEY, VALUE});
    writer.FeedEOF();
    string data = as_string(writer.Filled());
    ASSERT_TRUE(data.find(TIME1.String()) != string::npos);
    ASSERT_TRUE(data.find(KEY.String()) != string::npos);
    ASSERT_TRUE(data.find(VALUE) != string::npos);

    RONtStream reader{Stream::CLOSED, Stream::TMP};
    Op op{};
    ASSERT_OK(reader.FeedSpan(writer.Filled()));
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_TRUE(op.Values().IsEmpty());
    ASSERT_TRUE(op.Ref() == Uuid{LWW});
    ASSERT_TRUE(op.ID() == TIME1);
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_EQ(op.ID().String(), TIME2.String());
    ASSERT_EQ(op.Ref().String(), TIME1.String());
    Uuid ke;
    span32c ve;
    string ves;
    ASSERT_OK(op.Values().GetUuidString(ke, ve));
    ASSERT_EQ(ke, KEY);
    ASSERT_OK(SaveUtf8(ves, ve));
    ASSERT_EQ(ves, VALUE);
    ASSERT_EQ(reader.DrainOp(op).String(), ENDOFRAME.Ext(TERM::FACT).String());
}

TEST(TextFrame, optional_chars) {
    std::string TANGLED{
        "@1+A 256K 34, bigint +9223372036854775807,"
        " nospace 'юникод', @id {0}  3.1415;"};
    Op op;
    Uuid key;
    Integer ival;
    span32c cprval;
    string strval;
    double fval;

    RONtStream reader{Stream::CLOSED, Stream::TMP};
    ASSERT_OK(reader.FeedString(TANGLED));
    ASSERT_OK(reader.DrainOp(op));

    ASSERT_EQ(op.ID().String(), "1+A");
    ASSERT_NE(op.ID(), NIL);
    ASSERT_EQ(op.Ref(), NIL);
    ASSERT_OK(op.Values().GetUuidInteger(key, ival));
    ASSERT_EQ(key.String(), "256K");
    ASSERT_EQ(ival, 34);

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetUuidInteger(key, ival));
    ASSERT_EQ(key.String(), "bigint");
    ASSERT_EQ(ival, 9223372036854775807L);

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetUuidString(key, cprval));
    ASSERT_EQ(key.String(), "nospace");
    SaveUtf8(strval, cprval);
    ASSERT_EQ(strval, "юникод");

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetUuidFloat(key, fval));
    ASSERT_EQ(key.String(), "0");
    ASSERT_EQ(fval, 3.1415);
    ASSERT_EQ(op.ID().String(), "id");

    ASSERT_EQ(reader.DrainOp(op).String(), ENDOFRAME.Ext(TERM::FACT).String());
}

TEST(TextFrame, Spread) {
    string SPREAD{"@1-1-A ('\\tPaй\\n');"};
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    ASSERT_OK(reader.FeedString(SPREAD));
    Op op{};
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_EQ(op.Values().GetSingularString(), '\t');
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_EQ(op.Values().GetSingularString(), 'P');
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_EQ(op.Values().GetSingularString(), 'a');
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_EQ(op.Values().GetSingularString(), U'й');
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_EQ(op.Values().GetSingularString(), '\n');
    ASSERT_EQ(reader.DrainOp(op).String(), ENDOFRAME.Ext(1).String());
}

TEST(TextFrame, Incomplete) {
    string BAD{"B');"};  // in theory, this is an unfinished string
    RONtStream ront{Stream::CLOSED, Stream::TMP};
    Op op{};
    ASSERT_OK(ront.FeedString(BAD));
    ASSERT_EQ(ront.DrainOp(op).String(), NODATA.String());

    string BAD2{"@1-2-4"};
    RONtStream ront2{Stream::CLOSED, Stream::TMP};
    ASSERT_OK(ront2.FeedString(BAD2));
    ASSERT_EQ(ront2.DrainOp(op).String(), NODATA.String());
}

TEST(TextFrame, Bare) {
    string BARE{"1-2-4;"};
    RONtStream rr{Stream::CLOSED, Stream::TMP};
    Op op{};
    rr.FeedString(BARE);
    ASSERT_OK(rr.DrainOp(op));
    Uuid id;
    ASSERT_OK(op.Values().ReadAtoms().ReadUuid(id));
    ASSERT_EQ(id.String(), "1-2-4");
}

TEST(TextFrame, NoSpaces) {
    string DENSE{"1{2}3;"};
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    Op op{};
    reader.FeedString(DENSE);
    ASSERT_OK(reader.DrainOp(op));
    auto ar = op.Values().ReadAtoms();
    Integer i;
    Uuid u;
    ASSERT_OK(ar.ReadInteger(i));
    ASSERT_EQ(i, 1);
    ASSERT_OK(ar.ReadUuid(u));
    ASSERT_EQ(u.String(), "2");
    ASSERT_OK(ar.ReadInteger(i));
    ASSERT_EQ(i, 3);
}

TEST(TextFrame, signs) {
    std::string SIGNS{"@2:1 -1 , -1.2, +1.23, -1e+2, -2.0e+1;"};
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    reader.FeedString(SIGNS);
    Integer i;
    double f;
    Op op{};

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetInteger(i));
    ASSERT_EQ(i, -1);

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetFloat(f));
    ASSERT_EQ(f, -1.2);

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetFloat(f));
    ASSERT_EQ(f, 1.23);

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetFloat(f));
    ASSERT_EQ(f, -100.0);

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetFloat(f));
    ASSERT_EQ(f, -20);

    ASSERT_EQ(reader.DrainOp(op).String(), ENDOFRAME1.String());
}

TEST(TextFrame, size_limits) {
    std::string OVERLIMIT{" 1, 40000000000, 1000000000000000000001,"};
    RONtStream cur{Stream::CLOSED, Stream::TMP};
    cur.FeedString(OVERLIMIT);
    Op op{};
    ASSERT_OK(cur.DrainOp(op));
    ASSERT_OK(cur.DrainOp(op));
    ASSERT_EQ(cur.DrainOp(op).String(), BADSYNTAXI.String());
}

TEST(TextFrame, string_escapes) {
    RONtStream builder{Stream::CLOSED, Stream::TMP};
    std::string STR1{"'esc'"};
    std::string STR2{"=\r\n\t\\\u3040="};
    builder.FeedOp(Op{Uuid{"1+a"}, Uuid{"2+b"}, STR1, STR2});
    builder.FeedEOF();
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    reader.Create();
    reader.FeedSpan(builder.Filled());
    Op op{};
    ASSERT_OK(reader.DrainOp(op));
    span32c span1, span2;
    string s1, s2;
    ASSERT_OK(op.Values().GetStringString(span1, span2));
    ASSERT_OK(SaveUtf8(s1, span1));
    ASSERT_OK(SaveUtf8(s2, span2));
    ASSERT_EQ(s1, STR1);
    ASSERT_EQ(s2, STR2);
}

TEST(TextFrame, string_metrics) {
    std::string BADUTF8{"@id :ref 'bad string \x80';"};
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    reader.FeedString(BADUTF8);
    Op op{};
    ASSERT_EQ(reader.DrainOp(op).TrimExt().String(), BADSYNTAX.String());
}

TEST(TextFrame, terms) {
    std::string COMMAS{"@1+A:2+B 1, 2 ,\n,\t4   ,;"};
    RONtStream reader(Stream::CLOSED, Stream::TMP);
    reader.FeedString(COMMAS);
    int i = 0;
    Op op{};
    while (reader.DrainOp(op).Ok()) i++;
    ASSERT_EQ(i, 5);
}

TEST(TextFrame, defaults) {
    std::string FACT{
        "@12345+test :lww, @1234500001+test :12345+test 'key' 'value';"};
    RONtStream fact{Stream::CLOSED, Stream::TMP};
    fact.FeedString(FACT);
    RONtStream reser{Stream::CLOSED, Stream::TMP};
    Op op{};
    while (fact.DrainOp(op).Ok()) reser.FeedOp(op);
    reser.FeedEOF();
    std::string CORRECT{"@12345+test :lww,\n 'key' 'value';\n"};
    ASSERT_EQ(reser.Filled(), as_span8c(CORRECT));
    RONtStream reread{Stream::CLOSED, Stream::TMP};
    reread.FeedString(FACT);
    ASSERT_OK(reread.DrainOp(op));
    ASSERT_EQ(op.ID(), Uuid{"12345+test"});
    ASSERT_EQ(op.Ref(), Uuid{"lww"});
    ASSERT_OK(reread.DrainOp(op));
    ASSERT_EQ(op.ID(), Uuid{"1234500001+test"});
    ASSERT_EQ(op.Ref(), Uuid{"12345+test"});
}

TEST(TextFrame, span_spread) {
    std::string FACT{"@1iDEKK+gYpLcnUnF6 :1iDEKA+gYpLcnUnF6 ('abcd');"};
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    reader.FeedString(FACT);
    Op op{};
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_EQ(reader.DrainOp(op).TrimExt().String(), ENDOFRAME.String());
}
/*
TEST(TextFrame, syntax_errors) {
    std::string INVALID{"@line+ok\n:bad/"};
    Cursor cur{Slice{INVALID}};
    Status ok = cur.Next();
    std::string MSG{"syntax error at line 2 col 5 (offset 13)"};
    ASSERT_EQ(ok.comment(), MSG);
}

TEST(TextFrame, UTF16) {
    std::string PIKACHU{"'пикачу\\u0020ピカチュウ'!"};
    ASSERT_TRUE(PIKACHU.size()==36);
    Frame frame{PIKACHU};
    Cursor cur = frame.cursor();
    ASSERT_EQ(cur.Next(), OK);
    ASSERT_TRUE(HasValue(cur, STRING));
    Atom str = op.Get(2);
    std::string16 parsed;
    Readstd::string16(parsed, cur, str);
    ASSERT_TRUE(parsed==u"пикачу ピカチュウ");
    ASSERT_TRUE(parsed.size()==12);
}
*/

TEST(TextFrame, Spreads1) {
    std::string str{"@1lNBfg+0 :1lNBf+0 rm(3);\n"};
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    reader.Create();
    reader.FeedString(str);
    RONtStream writer{Stream::CLOSED, Stream::TMP};
    writer.Create();
    Op op{};
    Uuid u;

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetUuid(u));
    ASSERT_EQ(u.String(), "rm");
    ASSERT_OK(writer.FeedOp(op));

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetUuid(u));
    ASSERT_EQ(u.String(), "rm");
    ASSERT_OK(writer.FeedOp(op));

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetUuid(u));
    ASSERT_EQ(u.String(), "rm");
    ASSERT_OK(writer.FeedOp(op));

    ASSERT_EQ(reader.DrainOp(op).TrimExt().String(), ENDOFRAME.String());

    ASSERT_OK(writer.FeedEOF());
    auto str2 = writer.Filled();
    ASSERT_EQ(str2, as_span8c(str));

    //    string nospan{"@1lNBku+0 :max 1,\n 2,\n 3;\n"};
    //    Builder nob;
    //    nob.AppendAll(Frame{nospan});
    //    std::string no2;
    //    nob.EndFrame(no2);
    //    ASSERT_EQ(nospan, no2);
}

TEST(TextFrame, Spreads2) {
    std::string str{"@1lNBvg+0 :1lNBf+0 ('aㅂц');\n"};
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    reader.Create();
    reader.FeedString(str);
    RONtStream writer{Stream::CLOSED, Stream::TMP};
    writer.Create();
    Codepoint cp;
    Op op{};

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetSingularString(cp));
    ASSERT_EQ(cp, 'a');
    ASSERT_OK(writer.FeedOp(op));

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetSingularString(cp));
    ASSERT_EQ(cp, 0x3142);
    ASSERT_OK(writer.FeedOp(op));

    ASSERT_OK(reader.DrainOp(op));
    ASSERT_OK(op.Values().GetSingularString(cp));
    ASSERT_EQ(cp, 0x0446);
    ASSERT_OK(writer.FeedOp(op));

    ASSERT_EQ(reader.DrainOp(op).TrimExt(), ENDOFRAME);
    ASSERT_OK(writer.FeedEOF());
    auto str2 = writer.Filled();

    ASSERT_EQ(str, as_string(str2));
}

TEST(TextFrame, MakeSpread) {
    string separate{"@1lNBvg+0 :1lNBf+0 'a','ㅂ','ц';\n"};
    string spread{"@1lNBvg+0 :1lNBf+0 ('aㅂц');\n"};
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    RONtStream writer{Stream::CLOSED, Stream::TMP};
    reader.FeedString(separate);
    Op op{};
    while (reader.DrainOp(op).Ok()) ASSERT_OK(writer.FeedOp(op));
    writer.FeedEOF();
    auto condensed = writer.Filled();
    ASSERT_EQ(condensed, as_span8c(spread));
}

TEST(TextFrame, Copy) {
    std::vector<std::string> inputs{
        "'a\xf0\x8d\xbf\xbd';",
        "'\\uDD87\\uDF87',",
        "'\\udbecX',",
        " 4(' ')!",
        " e('\x06');",
        "'''!'''!(686),(686),",
        "@5:d,@o:d,@3:3,",
        ">1??",
        "'',,",
        " -0967022683410699646'';",
        " 7e37!",
        "\ne'',",
        " q    q(';'); ",
        "'����eeeeeeeeee'?5e	q	wq	w,",
        " 2222222222222222222222222222222222222222e222222222222222?",
        " 4e0,",
        " 2.0,",
        " 1e+6!",
    };
    for (auto& input : inputs) {
        RONtStream reader{Stream::CLOSED, Stream::TMP};
        reader.FeedString(input);
        RONtStream writer{Stream::CLOSED, Stream::TMP};
        Op op{};
        bool some = false;
        while (reader.DrainOp(op).Ok()) {
            writer.FeedOp(op);
            some = true;
        }
        if (!some) continue;
        writer.FeedEOF();
        Op op1{}, op2{};
        RONtStream reader2{Stream::CLOSED, Stream::TMP};
        reader2.FeedString(input);
        RONtStream copy_reader{Stream::CLOSED, Stream::TMP};
        copy_reader.FeedSpan(writer.Filled());
        while (reader2.DrainOp(op1).Ok() && copy_reader.DrainOp(op2).Ok())
            ASSERT_TRUE(op1.IsEqual(op2));
        ASSERT_FALSE(copy_reader.DrainOp(op).Ok());
    }
}

TEST(RONt, FuzzLoop) {
    std::vector<std::string> inputs{
        "@7+m(',');",
        "@~,:~;",
        "@2+E,@0,:2+E;",
        "0.5?",
        "@~~~~~-e-0,,:~~~~~-e-0,;",
        "@z-H-e I(61),I(8)2",
        "@~~~~z-t-5,('~~~\xea)",
        "@0+T,'''e';",
        "@r+u,@5rrr^uu:E+222,E,E;",
        "@2 -1,@0 -1,@",
        "@C  t,;",
        "@C ' ','",
        "@K,@K,z",
        "z,c;",
        "5!",
        "('\xc1\x9c\\xdb');",
        "('\xf4\xa7\xa6\xa6\xf4\xf4');",
        "@2('7B!\t\0",
        "42 2FFFFFFFFFFF^4!",
        "f5 2E566,",
        "2c .2;",
        "07 >1e3,,",
        "('\\01');",
        "0a 4^4E-2,",
        ",'U",
    };
    // 0 RONt --> 1 RONc --> 2 RONt --> 3 RONc
    for (auto& inp : inputs) {
        RONtStream t01{Stream::CLOSED, Stream::TMP};
        t01.FeedString(inp);
        RONtStream t12{Stream::CLOSED, Stream::TMP};
        Op op{};
        Ops ronc{};
        WHILE(loads, t01.DrainOp(op)) {
            ASSERT_OK(t12.FeedOp(op));
            ronc.push_back(op);
        }
        if (ronc.empty()) continue;
        t12.FeedEOF();
        auto res = as_string(t12.Filled());
        RONtStream t23{Stream::CLOSED, Stream::TMP};
        t23.FeedSpan(t12.Filled());
        auto i = ronc.begin();
        WHILE(moreloads, t23.DrainOp(op)) {
            //            if (i >= ronc.end()) {
            //                cerr << "ORIG: " << inp << "\nFACT: " << res <<
            //                "\n";
            //            }
            ASSERT_TRUE(i < ronc.end());
            if (!op.IsEqual(*i)) {
                cerr << "ORIG: " << inp << "\nFACT: " << res << "\n";
                cerr << "[" << i->Values().Pattern() << "<>"
                     << op.Values().Pattern() << "]\n";
                cerr << i->Values().GetSingularFloat() << '\n';
                cerr << op.Values().GetSingularFloat() << '\n';
            }
            ASSERT_TRUE(op.IsEqual(*i));
            ++i;
        }
        //        if (i != ronc.end()) {
        //            cerr << "ORIG: " << inp << "\nFACT: " << res << "\n"
        //                 << moreloads.String();
        //        }
        ASSERT_TRUE(i == ronc.end());
    }
}

TEST(RONt, ResumedFuzzLoop) {  // 0RONt>1RONc(>2RONt>3RONc)(>4RONv>5RONc)
    std::vector<std::string> inputs{
        "@1-2-lww'striue';", "@s+3'',,", "@v+a,0'','','',",
        "@v1C6-x-x,se 3em,", "40 3 5;",  "e a,",
        "4 _,@8+3 (13 ",     "3!",       "?",
    };
    for (auto& inp : inputs) {
        RONtStream t01{Stream::CLOSED, Stream::TMP};
        RONtStream writer12{Stream::CLOSED, Stream::TMP};
        RONtStream reader23{Stream::CLOSED, Stream::TMP};
        RONvStream writer14{};
        ASSERT_OK(writer14.Open(-1, Stream::TMP));
        Ops ronc1{};
        Op op{};
        while (t01.DrainOp(op).Ok()) {
            writer12.FeedOp(op);
            ronc1.push_back(op);
            ASSERT_OK(writer14.FeedOp(op));
        }
        if (ronc1.empty()) continue;
        writer12.FeedEOF();
        writer14.FeedEOF();

        auto i = ronc1.begin();
        uint8_t buf[5];
        buf[4] = 0;
        while (!writer12.Filled().empty()) {
            span8 b{buf, 4};
            ASSERT_OK(writer12.DrainToSpan(b));
            ASSERT_OK(reader23.FeedSpan(b));
            WHILE(r23, reader23.DrainOp(op)) {
                ASSERT_TRUE(i < ronc1.end());
                ASSERT_TRUE(op.IsEqual(*i));
                ++i;
            }
            ASSERT_TRUE(r23 == OK || r23.Is(ENDOFRAME) || r23.Is(NODATA));
            // TODO status
        }
        ASSERT_TRUE(i == ronc1.end());

        RONvStream reader45{};
        ASSERT_OK(reader45.Open(-1, Stream::TMP));
        auto j = ronc1.begin();
        span8c data14 = writer14.Filled();
        reader45.FeedSpan(data14);
        WHILE(t45, reader45.DrainOp(op)) {
            ASSERT_TRUE(op.IsEqual(*j));
            ++j;
        }
        ASSERT_TRUE(j == ronc1.end());
    }
}

/*
TEST(TextFrame, Compare) {
    ASSERT_TRUE(
        Frame::IsEqual(Frame{"@1-2-yarn 3;"}, Frame{"@1-2-yarn :0 =3 ;"}));
    ASSERT_TRUE(
        Frame::IsEqual(Frame{"@1-2-yarn '3';"}, Frame{"@1-2-yarn :0 '3' ;"}));
    ASSERT_FALSE(
        Frame::IsEqual(Frame{"@1-2-yarn '3';"}, Frame{"@1-2-yarn :0 =3 ;"}));
    ASSERT_FALSE(
        Frame::IsEqual(Frame{"@1-2-yarn '3';"}, Frame{"@1-2-yarn :0 '4' ;"}));
}
*/

/*TEST(RONc, Boxes) {
    string full = "'value';";
    RONtReader cursor{full};
    ASSERT_OK(cursor.DrainOp(op));
    auto reader = cursor.op.Values().ReadBoxes();
    Tuple::Box box;
    ASSERT_OK(reader.ReadBox(box));
    auto sub1 = box.SubBox(0, 2);
    auto sub2 = box.SubBox(2);
    Op newop{};
    newop.Values().PushBox(sub1);
    newop.Values().PushBox(sub2);
    RONtWriter writer{};
    writer.FeedOp(newop);
    writer.FeedEOF();
    string result = writer.writer.Release();
    ASSERT_EQ(result, " 'va' 'lue';\n");
}*/

TEST(RONc, Copy) {
    string ront = "@1-2-test :0-1-test 1 'два' 3.0;\n";
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    reader.FeedString(ront);
    RONtStream writer{Stream::CLOSED, Stream::TMP};
    Op op{};
    while (reader.DrainOp(op).Ok()) writer.FeedOp(op);
    writer.FeedEOF();
    ASSERT_EQ(ront, as_string(writer.Filled()));
}

TEST(RONt, Blob) {
    string ront =
        "@blob :12345 5!\n"
        "test\n"
        "\n";
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    Op op{};
    reader.FeedString(ront);
    ASSERT_OK(reader.DrainOp(op));
    ASSERT_EQ(op.Values().FirstBoxDesc().coded_, BoxDescriptor::INT1LAST);
    ASSERT_EQ(reader.DrainOp(op).String(), ENDOFRAME2.String());
    // FIXME blob ASSERT_TRUE(reader.rstate.Blob() == as_span8c("test\n"));
}

TEST(RONt, PipeAMillion) {
    static constexpr int COUNT = 1 << 20;
    TmpDir tmp{"RONtMillion"};
    auto one = Uuid::Yolo(1, 1, TEST);
    auto zero = Uuid::Yolo(0, 0, TEST);

    string pipe_path{"pipe"};
    RONtStream writer{Stream::CLOSED, Stream::TMP};
    RONtStream reader{Stream::CLOSED, Stream::TMP};
    //    ASSERT_OK(writer.Create(pipe_path, Stream::SHARED_WRITE));
    //    ASSERT_OK(reader.Open(pipe_path, Stream::SHARED_READ));

    Op op{zero, Uuid{"some"}};
    for (int i = 0; i < COUNT; ++i) {
        op.Values().Clear();
        op.Values().PushInteger(i);
        ASSERT_OK(writer.FeedOp(op));
        op.NextInEvenChain();
    }
    ASSERT_OK(writer.FeedEOF());

    int i = 0;
    while (i < COUNT) {
        auto l = std::min(writer.FilledSize(), 10UL);
        auto w = writer.Filled().subspan(0, l);
        //        write(STDOUT_FILENO, w.data(), l);
        auto s = reader.FeedSpan(w);
        writer.Drained(l);

        ASSERT_TRUE(s == OK || s == NODATA);
        Status r;
        Op copy{};
        while (OK == (r = reader.DrainOp(copy))) {
            ASSERT_EQ(copy.ID().HomeIndex(), i);
            ASSERT_EQ(copy.ID().YarnIndex(), i);
            ASSERT_EQ(copy.ID().Origin(), TEST);
            Integer j;
            auto r = copy.Values().ReadAtoms();
            ASSERT_OK(r.ReadInteger(j));
            ASSERT_EQ(j, i);
            ++i;
        }
        //        cerr<<r.String()<<'\n';
        ASSERT_TRUE(r.Is(ENDOFRAME) || r.Is(NODATA));  //?FIXME
    }
}

TEST(RONt, TwoSpreads) {
    string test1{"@1-1-A ('ab')"}, test2{","}, test3{" ('cd');"};
    RONtStream reader{};
    reader.Create();
    reader.FeedString(test1);
    ASSERT_EQ(reader.DrainOp(), NODATA);
    reader.FeedString(test2);
    ASSERT_OK(reader.DrainOp());
    ASSERT_EQ(reader.Values().GetSingularString(), 'a');
    ASSERT_OK(reader.DrainOp());
    ASSERT_EQ(reader.Values().GetSingularString(), 'b');
    reader.FeedString(test3);
    ASSERT_OK(reader.DrainOp());
    ASSERT_EQ(reader.Values().GetSingularString(), 'c');
    ASSERT_OK(reader.DrainOp());
    ASSERT_EQ(reader.Values().GetSingularString(), 'd');
}

TEST(RONt, ShovelATon) {
    static constexpr int COUNT = 0xD800;
    TmpDir tmp{"RONtTon"};
    auto one = Uuid::Yolo(1, 1, TEST);
    auto zero = Uuid::Yolo(0, 0, TEST);
    RONtStream writer{Stream::CLOSED, Stream::TMP};
    RONtStream reader{Stream::CLOSED, Stream::TMP};

    Op op{zero, Uuid{"some"}};
    for (int i = 0; i < COUNT; ++i) {
        op.Values().Clear();
        op.Values().PushCodepoint(i);
        ASSERT_OK(writer.FeedOp(op));
        op.NextInEvenChain();
    }
    ASSERT_OK(writer.FeedEOF());

    int i = 0;
    uint8_t buf[4];
    string32 j;
    while (i < COUNT) {
        span8 b{buf, 4};
        ASSERT_OK(writer.DrainToSpan(b));
        span8c c{buf, 4 - b.size_bytes()};
        ASSERT_OK(reader.FeedSpan(c));
        Status rs;
        Op copy{};
        while (OK == (rs = reader.DrainOp(copy))) {
            ASSERT_EQ(copy.ID().HomeIndex(), i);
            ASSERT_EQ(copy.ID().YarnIndex(), i);
            ASSERT_EQ(copy.ID().Origin(), TEST);
            auto r = copy.Values().ReadAtoms();
            ASSERT_OK(r.ReadString(j));
            ASSERT_EQ(j.size(), 1);
            ASSERT_EQ(j.front(), i);
            ++i;
        }
        auto good = rs == NODATA || rs.TrimExt() == ENDOFRAME;
        if (!good) {
            cerr << rs.String() << " " << copy.ID().String() << "\n";
        }
        ASSERT_TRUE(good);
    }
}

TEST(RONc, BoxSugar) {
    auto tuple = Tuple::As("1 2 3");
    auto box = tuple.FirstBox();
    ASSERT_EQ(box.Size(), 3);
    ASSERT_EQ(box[0], 1L);
    ASSERT_EQ(box[1], 2L);
    ASSERT_EQ(box[2], 3L);
    auto ft = Tuple::As("1.0 2.0");
    auto fb = ft.FirstBox();
    ASSERT_TRUE(fb[0] == 1.0);
    auto ut = Tuple::As("one two");
    auto ub = ut.FirstBox();
    ASSERT_TRUE(ub[1] == Uuid{"two"});
}

// TODO TEST(Tuple, Coerce) {
//    Tuple t{};
//    ASSERT_OK(t.CoerceAtom("1"));
//    ASSERT_OK(t.CoerceAtom("test"));
//    ASSERT_OK(t.CoerceAtom("строка"));
//    Op op{};
//    ASSERT_OK(Op::Parse(op, "1 test 'строка';"));
//    ASSERT_TRUE(t == op.Values());
//}

#include "../uuid.hpp"

#include <cstdio>

#include "test.hpp"

using namespace ron;
using namespace std;

TEST(Word, BitLayout) {
    ASSERT_EQ(sizeof(Word), 8);

    Word word;
    ASSERT_OK( Uuid::ParseWord(word, as_span8c("word")) );
    ASSERT_EQ(word, 1077444979162349568);

    Uuid tag{"tag^orig"};
    Uuid name{"LongName"};
    Uuid yolo{"1-2-yolo"};
    Uuid lamport{"3+lamport"};
    ASSERT_TRUE(yolo.IsEvent());
    ASSERT_TRUE(lamport.IsEvent());
    ASSERT_FALSE(name.IsEvent());
    ASSERT_FALSE(tag.IsEvent());
}

TEST(Uuid, Delta) {
    auto one = Uuid::Yolo(1, 5, TEST);
    auto two = Uuid::Yolo(5, 1, TEST);
    Uuid delta{}, two2{};
    ASSERT_OK(Uuid::Delta(delta, two, one));
    ASSERT_OK(Uuid::ApplyDelta(two2, delta, one));
    ASSERT_EQ(two, two2);
    ASSERT_TRUE(delta.HomeIndex() < 12);
    ASSERT_TRUE(delta.YarnIndex() < 12);

    auto u1 = Uuid::Lamport(4, TEST);
    auto u2 = Uuid::Lamport(8, TEST);
    Uuid ud{}, u2b{};
    ASSERT_OK(Uuid::Delta(ud, u2, u1));
    ASSERT_OK(Uuid::ApplyDelta(u2b, ud, u1));
    ASSERT_EQ(u2, u2b);
    ASSERT_EQ(ud.Variant(), Uuid::DELTA);

    Uuid d0{}, ct{};
    ASSERT_OK(Uuid::Delta(d0, LWW_RDT, LWW_RDT));
    ASSERT_EQ(d0.first, 0);
    ASSERT_OK(Uuid::ApplyDelta(ct, d0, LWW_RDT));
    ASSERT_EQ(ct, LWW_RDT);
}

TEST(Uuid, MicroCache) {
    auto one = Uuid::Yolo(1, 5, TEST);
    auto two = Uuid::Yolo(5, 1, TEST);
    Uuid d, t2;
    MicroCache cache{};
    cache.Store(one);
    ASSERT_OK(cache.MakeDelta(d, two));
    ASSERT_OK(cache.ResolveDelta(t2, d));
    ASSERT_EQ(t2, two);

    ASSERT_FALSE(cache.MakeDelta(d, Uuid::Yolo(1, 2, CT_WORD)).Ok());

    cache.Store(LWW_RDT);
    Uuid cd, c2;
    ASSERT_OK(cache.MakeDelta(cd, LWW_RDT));
    ASSERT_OK(cache.ResolveDelta(c2, cd));
    ASSERT_EQ(cd.first, 0);
    ASSERT_EQ(c2, LWW_RDT);
}

TEST(Uuid, String) {
    auto yolo = Uuid::Yolo(65, 63, TEST);
    ASSERT_EQ(yolo.String(), "11-~-test");

    auto lamport = Uuid::Lamport(1UL << 54, TEST);
    ASSERT_EQ(lamport.String(), "1+test");
    auto lamport2 = Uuid::Lamport(1, TEST);
    ASSERT_EQ(lamport2.String(), "0000000001+test");

    Uuid name{YARN_RDT};
    ASSERT_EQ(name.String(), "yarn");
}

/*TEST(Word, Case) {
    ASSERT_TRUE(Uuid::NIL.Value().base64_case()==NUMERIC);
    ASSERT_TRUE(Uuid{"2134"}.Value().base64_case()==NUMERIC);
    ASSERT_TRUE(Uuid{"ABC~DEFZ"}.Value().base64_case()==CAPS);
    ASSERT_TRUE(Uuid{"Abc_Def"}.Value().base64_case()==CAMEL);
    ASSERT_TRUE(Uuid{"abc_xyz"}.Value().base64_case()==SNAKE);
}*/

/*TEST (Uuid, All) {
    ASSERT_TRUE(sizeof(Word)==8);
    ASSERT_TRUE(sizeof(Uuid)==16);

    Uuid u1{"0000000001"};
    ASSERT_TRUE(u1.Value()==1);
    ASSERT_TRUE(u1.String()=="0000000001");
    //ASSERT_TRUE(u1.variant()==0); // FIXME
    ASSERT_TRUE(u1.Type()()==UUID::NAME);

    Uuid test{"test"};
    std::string test_str = test.String();
    ASSERT_TRUE(test_str=="test");

    static const Uuid SUBS_CF_UUID{1007006897032658944L, 0};
    ASSERT_TRUE(SUBS_CF_UUID.String()=="subs");

    Uuid zeros{"abc000"};
    ASSERT_TRUE(zeros.String()=="abc");

    Uuid one = Uuid::Time(1,Word{"origin"});
    ASSERT_TRUE(one.Type()()==UUID::TIME);
    ASSERT_TRUE(one.String()=="0000000001+origin");
    Uuid two = one.inc();
    ASSERT_TRUE(two.String()=="0000000002+origin");

    ASSERT_TRUE(one==one);
    ASSERT_TRUE(two>one);
    ASSERT_TRUE(one<two);
    ASSERT_TRUE(one!=two);

}*/

TEST(Uuid, Parse) {
    Uuid u;
    ASSERT_OK(Uuid::ParseUuid(u, as_span8c("0")));
    EXPECT_EQ(u, NIL);
    EXPECT_EQ(u.String(), "0");

    ASSERT_OK(Uuid::ParseUuid(u, as_span8c("~")));
    EXPECT_EQ(u.first, 63UL << 54U);
    EXPECT_EQ(u.String(), "~");

    ASSERT_OK(Uuid::ParseUuid(u, as_span8c("rga^scoped")));
    EXPECT_EQ(u.Value(), 985043671231496192UL);
    EXPECT_EQ(u.Origin(), 1001997360592584704UL);
    EXPECT_EQ(u.second, 1001997360592584704UL | TAG_FLAGS);
    EXPECT_EQ(u.Version(), TAG);
    EXPECT_EQ(u.String(), "rga^scoped");

    ASSERT_OK(Uuid::ParseUuid(u, as_span8c("1-2-origin")));
    EXPECT_EQ(u.HomeIndex(), 1);
    EXPECT_EQ(u.YarnIndex(), 2);
    EXPECT_EQ(u.Origin(), 934134888913698816UL);
    EXPECT_EQ(u.second, 934134888913698816UL | YOLO_FLAGS);
    EXPECT_EQ(u.String(), "1-2-origin");

    ASSERT_OK(Uuid::ParseUuid(u, as_span8c("LongBeautifulName")));
    EXPECT_EQ(u.first, 392880462283382317UL);
    EXPECT_EQ(u.second, 772861538412527616UL);
    EXPECT_EQ(u.String(), "LongBeautifulName");

    ASSERT_OK(Uuid::ParseUuid(u, as_span8c("000000001+origin")));
    EXPECT_EQ(u.Value(), 64);
    EXPECT_EQ(u.Origin(), 934134888913698816UL);
    EXPECT_EQ(u.String(), "000000001+origin");
}

TEST(Uuid, OutOfRange) {
    ASSERT_EQ(Uuid{"longin-dex-is_bad"}, FATAL);
    ASSERT_EQ(Uuid{"TooLongTime+Lamport"}, FATAL);
    ASSERT_EQ(Uuid{"Scoped^Long_Origin"}, FATAL);
    ASSERT_EQ(Uuid{"ThisOneIsReallyTooLong"}, FATAL);
    ASSERT_NE(Uuid{"12345-67890-GoodOrigin"}, FATAL);
}

TEST(Uuid, Order) {
    ASSERT_EQ(Uuid::Yolo(1, 2, TEST), Uuid::Yolo(1, 2, TEST));
    ASSERT_GE(Uuid::Yolo(1, 2, TEST), Uuid::Yolo(1, 2, TEST));
    ASSERT_GT(Uuid::Yolo(2, 3, TEST), Uuid::Yolo(1, 2, TEST));
    ASSERT_GT(Uuid::Yolo(2, 3, TEST), Uuid::Yolo(1, 4, TEST));
    ASSERT_LE(Uuid::Yolo(2, 3, TEST), Uuid::Yolo(3, 4, TEST));
    ASSERT_LT(Uuid::Yolo(2, 3, TEST), Uuid::Yolo(2, 4, TEST));
}

TEST(RFC, TRY) {
    TRY(ni, NOTIMPLTD);
    if (ni.Is(NOTIMPLTD)) {
        ASSERT_TRUE(true);
    } else {
        ASSERT_FALSE(true);
    }
    if (ni.Ok()) {
        ASSERT_FALSE(true);
    }
    bool ok = false;
    TRY(nf, NOTFOUND);
    if (nf.Is(NOTIMPLTD)) {
        ASSERT_FALSE(true);
    } else if (!nf.Ok()) {
        ok = true;
    }
    ASSERT_TRUE(ok);
    ASSERT_DEATH(MUST(false, "oopsie"), "oopsie");
}
